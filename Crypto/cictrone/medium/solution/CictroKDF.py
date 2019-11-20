from Crypto.Cipher import AES
from base64 import b64encode, b64decode

def getbit(val, index):
    return (val >> index) & 1

def setbit(val, index, bit):
    rv = val >> (index + 1)
    rv <<= 1
    rv |= bit
    rv <<= index
    lower = val ^ ((val >> index) << index)
    return rv | lower

def add_bytes(first, second):
    return first ^ second ^ 56

def bit_array_to_int(bits):
    val = 0
    for bit in bits:
        val |= bit
        val <<= 1
    return val >> 1

def int_to_bit_array(num):
    bits = []
    bit_padding = 8 - num.bit_length()
    while num != 0:
        bit = num & 1
        bits.insert(0, bit)
        num >>= 1
    for i in range(bit_padding):
        bits.insert(0, 0)
    return bits

def round_func(byte, round_num):
    byte = setbit(byte, round_num % 8, 1)


    bits = int_to_bit_array(byte)
    new_bits = []
    new_bits.append(bits[3])
    new_bits.append(bits[2] ^ bits[5])
    new_bits.append(bits[6])
    new_bits.append(bits[1] ^ bits[0])
    new_bits.append(bits[0])
    new_bits.append(bits[7] ^ bits[2])
    new_bits.append(bits[4])
    new_bits.append(bits[5])
    return bit_array_to_int(new_bits)

def expand(hash_list):
    temp = hash_list[0] ^ hash_list[-1]
    temp2 = hash_list[1] ^ hash_list[-2]
    hash_list.append(round_func(temp ^ temp2, 101))
    return hash_list

def kdf(message):
    output_str = "hash"
    output = [ord(output_str[0]), ord(output_str[1]), ord(output_str[2]), ord(output_str[3])]
    for i in range(len(message)):
        byte = message[i]
        byte_num = ord(byte)
        byte_orig = byte_num
        # begin round
        for j in range(100):
            byte_num = round_func(byte_num, j)
            byte_num = add_bytes(byte_orig, byte_num)
        output[i % 4] = add_bytes(output[i % 4], byte_num)
        # end round
    for i in range(15):
        output = expand(output)
    b = b''
    for o in output:
        b += int.to_bytes(o, 1, 'big')
    return b[3:]

if __name__ == '__main__':
    key = kdf("Are you silly? I'm still gunna send it!!")
    print(key)
    cipher = AES.new(key, AES.MODE_ECB)
    ctxt = cipher.encrypt('i am a person who dos the crypto')
    b64_ctxt = b64encode(ctxt)
    print(b64_ctxt.decode())
    print(cipher.decrypt(b64decode(b64_ctxt)))
