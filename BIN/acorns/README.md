Description: Can you read this code and decipher the flag?

Solution: RITSEC{hey_nice_you_can_reverse_ARM_assembly}
100
It's a snipper of arm code that decodes a string. Teams just have to figure out what the decoding routine is.
for char in string:
	char = (char ^ 0x21) - 21
