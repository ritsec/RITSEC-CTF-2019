import json
from multiprocessing import Pool

from scipy import stats

from evil_accountant.sbox import sbox_lookup


def load_jsoned_traces(filename):
    """same as above but without key and also JSON."""
    with open(filename, "r") as input_file:
        traces = json.load(input_file)

    return (
        [43, 126, 21, 22, 40, 174, 210, 166, 171, 247, 21, 136, 9, 207, 79, 60],
        traces["traces"],
        traces["plaintexts"],
    )


def power_model(intermediate_value):
    # Hamming weight is used as power model - more 1s, more power usage
    return bin(intermediate_value).count("1")


def get_subkey_guess_correlation(subkey, byte_index, traces, plaintexts):
    # A subkey is one byte
    assert subkey >= 0 and subkey < 256
    # The byte index is the index of the byte of key that is being guessed
    assert byte_index >= 0 and byte_index < 16

    # Store the output of our power usage model for each plaintext/trace pair
    modeled_usage = []

    # Compute the intermediate value for every plaintext and store the results of the
    # power usage model
    for plaintext in plaintexts:
        addkey_output = plaintext[byte_index] ^ subkey
        subbytes_output = sbox_lookup(addkey_output)
        modeled_usage.append(power_model(subbytes_output))

    # Find the correlation coefficient between the modeled power usage and the actual
    # power usage for each data point in the traces (e.g., for point 1 in all traces,
    # for point 2, etc.).
    correlations = []
    for data_point_idx in range(len(traces[0])):
        measurements = [trace[data_point_idx] for trace in traces]
        coefficient = stats.pearsonr(modeled_usage, measurements)
        correlations.append(abs(coefficient[0]))

    # We only return the maximum correlation coefficient of each data point. This
    # eliminates noise in the trace from parts of the encryption unrelated to the
    # intermediate value we're attacking.
    return max(correlations)


def get_correct_subkey_byte(byte_index, traces, plaintexts):
    # The byte index is the index of the byte of key that is being guessed
    assert byte_index >= 0 and byte_index < 16

    # Find the subkey guess with the highest coefficent
    max_coefficent = 0
    best_subkey_guess = 0
    for subkey_guess in range(256):
        guess_coefficient = get_subkey_guess_correlation(
            subkey_guess, byte_index, traces, plaintexts
        )
        if guess_coefficient > max_coefficent:
            max_coefficent = guess_coefficient
            best_subkey_guess = subkey_guess

    return best_subkey_guess, max_coefficent


def get_key(traces, plaintexts):
    key = []

    # Split up the key into 16 different 1-byte subkeys that will be determined
    # individually.
    with Pool(processes=8) as pool:
        args = [(x, traces, plaintexts) for x in range(16)]
        key = pool.starmap(get_correct_subkey_byte, args)
        print(f'Coefficients: {["%4.3f" % x[1] for x in key]}')
        print(f'Key guess:    {[hex(x[0]) for x in key]}')


def main():
    # Load the traces
    key, traces, plaintexts = load_jsoned_traces("traces.json")
    print(f'Actual key:   {[hex(x) for x in key]}')
    get_key(traces, plaintexts)
    print(f'Actual key:   {[hex(x) for x in key]}')


if __name__ == "__main__":
    main()
