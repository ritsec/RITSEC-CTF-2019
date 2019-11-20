lPhAbEtIcAl Challenge
## RITSEC CTF 2019
Author: Impos73r

Flag: *RITSEC{YouAlphabetizedYourNumbers}*
Encoded Flag: *59:87:57:51:85:80:{:40:50:56:08:82:58:81:08:18:85:57:87:48:85:88:40:50:56:59:15:56:11:18:85:59:51:}*
## Hints:
1. Things go from 0-100 REAL QUICK
2. Alphabetize Your Numbers
3. (Solves It) Spell the numbers out from 0-100, then sort them into alphabetical order, from there just decode the flag and you should be good to do.

## How To Solve: 
1. Spell out all the numbers from 1-100, (One, Two, Three, Four....)
2. Alphabetize the list
3. Link the first 26 numbers to the characters in the alphabet (The list I used is below)
4. Then the next 10 to the digits (1,2,3...0) 
5. Each two digit combo in the flag can then be decoded with the contents in the dict.
6. Then you'll get the flag

The Dict I had was:
encoding = {
    "A": "08",
    "B": "18",
    "C": "80",
    "D": "88",
    "E": "85",
    "F": "84",
    "G": "89",
    "H": "81",
    "I": "87",
    "J": "86",
    "K": "83",
    "L": "82",
    "M": "11",
    "N": "15",
    "O": "50",
    "P": "58",
    "Q": "54",
    "R": "59",
    "S": "51",
    "T": "57",
    "U": "56",
    "V": "53",
    "W": "52",
    "X": "05",
    "Y": "40",
    "Z": "48",
    "1": "45",
    "2": "44",
    "3": "49",
    "4": "41",
    "5": "47",
    "6": "46",
    "7": "43",
    "8": "42",
    "9": "04",
    "0": "14"
}
