from argparse import ArgumentParser
parser = ArgumentParser()
parser.add_argument('-f', '--file', help='Specify an alternate flag file', default='flag.txt')
args = parser.parse_args()

# Radical of an Integer Sequence up to 36th term
bases = [1, 2, 3, 2, 5, 6, 7, 2, 3, 10, 11, 6, 13, 14, 15, 2, 17, 6, 19, 10, 21, 22, 23, 6, 5, 26, 3, 14, 29, 30, 31, 2, 33, 34, 35, 6]

# representations of numbers, index is value
representations = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
                   'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z']

try:
    with open(args.file, 'r') as f:
        flag = f.readline().strip()
except FileNotFoundError:
    print("flag.txt file not found!")

def ord_to_newbase(char_ord, char_idx):
    """
    convert char to the new base based on bases list entry at same index
    """
    base = bases[char_idx]
    # The algorithm doesn't work on base 1 because it's special
    if base == 1:
        return "".join("1" for i in range(char_ord))
    
    # Kinda jank but oh well, spits out encoded number but backwards
    # mod by power of next index, current digit is result divided by current index
    place = 0
    encoded_flag = ""
    while char_ord > 0:
        val = char_ord % (base**(place + 1))
        rep = representations[val // base**place]
        char_ord -= val
        encoded_flag += rep
        place += 1

    return encoded_flag[::-1]


encoded_char_list = [ord_to_newbase(ord(flag[i]), i) for i in range(len(flag))]
encoded_chars = ':'.join(encoded_char_list)
print(encoded_chars)
