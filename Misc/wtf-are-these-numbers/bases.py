from argparse import ArgumentParser
parser = ArgumentParser()
parser.add_argument('f', help='File to read encoded flag from')
args = parser.parse_args()

# this is the Radical of an Integer sequence up to the 18th term
bases = [1, 2, 3, 2, 5, 6, 7, 2, 3, 10, 11, 6, 13, 14, 15, 2, 17, 6, 19, 10, 21, 22, 23, 6, 5, 26, 3, 14, 29, 30, 31, 2, 33, 34, 35, 6]

# list of all representations of numbers, index corresponds to value
representations = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
                   'K', 'L', 'M', 'N', '0', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z']

try:    
    file = open(args.f)
except FileNotFoundError:
    print(f'file {args.f} not found!')

encoded = file.readline()

encoded_list = encoded.strip().split(':')


def calc_num(base, reversed_old_number):
    """
    Calculate the base 10 representation of the character
    :param base: base encoded in
    :param reversed_old_number: number with digits reversed (so loop is easier)
    :return: base 10 ascii character code
    """
    n = 0
    sum = 0
    for digit in reversed_old_number:
        sum += ((base**n) * int(representations.index(digit)))
        n += 1
    return sum

print("".join(chr(calc_num(bases[i], code[::-1])) for i, code in zip(range(len(encoded_list)), encoded_list)))
