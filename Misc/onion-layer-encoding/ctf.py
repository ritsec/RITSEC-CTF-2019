"""
Author: Sunggwan Choi 
Description: MISC 100 challenge for RITSEC CTF 2019. 

Note to Q: Q, Just simply run the script, and deploy the "onionlayerencoding.txt". 

"""

import subprocess
import random 
import sys
import base64
import string 


def creation(bytestring, layer):
    """
    Description: From the bytestring input, perform base16, 32, 64 encoding randomly for 50 times. 
    Param:
        (bytes) bytestring: Original flag in bytes format. This flag will get encoded. 
        (int) layer: Number of times the onion layer will encode the original flag 
    Return:
        (bytes) bytestring: Onion encoded flag. 
    """
    for i in range(layer):
        rand = random.randint(0,2)

        if rand == 0:
            print("\n [" + str(i) + "] [DEBUG] base16 encoding... ")
            bytestring = base64.b16encode(bytestring)
            #print(bytestring)  # [DEBUG]

        if rand == 1:
            print("\n [" + str(i) + "] [DEBUG] base32 encoding...")
            bytestring = base64.b32encode(bytestring)
            #print(bytestring)  # [DEBUG]


        if rand == 2:
            print("\n [" + str(i) + "] [DEBUG] base64 encoding...")
            bytestring = base64.b64encode(bytestring)
            #print(bytestring)  # [DEBUg]


    return bytestring 

def check(string):
    """
    Description: Function which checks if the string is a base16, 32, 64 string. 
    Param:
        (str) string: Payload string to check if it's encoded in base16,32,64.
    Return: 
        (int) 0:  Indicating that this string is base16 
        (int) 1:  Indicating that this string is base32
        (int) 2:  Indicating that this string is base64 
    """

    base16 = set("0123456789ABCDEFabcdef")
    base32 = set("ABCDEFGHIJKLMNOPQRSTUVWXYZ234567=")
    base64 = set("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+/=")

    if all(x in base16 for x in string):
        print("[+] Peeling off... Onion: base16")
        return 0

    elif all(x in base32 for x in string):
        print("[+] Peeling off... Onion: base32")
        return 1

    else:
        print("[+] Peeling off... Onion: base64")
        return 2


def solution(bytestring, layer):
    """
    Description: Function which solves the Onion Layer Encoding challenge 
    Param:
        (bytes) bytestring: Onion layer encoded payload. This will be solved. 
        (int) layer: Number of times the solution function will peel off base16,32,64 onion layer 
    Return:
        None 
    Note: During the solution, the funciton will automatically exit upon finding the flag format.

    """
    
    for i in range(layer):
        string = bytestring.decode('utf-8')

        encoding = check(string)

        if encoding == 0:
            bytestring = base64.b16decode(bytestring)
            #print(bytestring)   # DEBUG

        elif encoding == 1:
            bytestring = base64.b32decode(bytestring)
            #print(bytestring)   # DEBUG 

        else:
            bytestring = base64.b64decode(bytestring)
            #print(bytestring)   # DEBUG 

        if "RITSEC{" in bytestring.decode('utf-8'):
            print("\n[!!!] ============ Flag Found ============ [!!!]\n")
            print(bytestring.decode('utf-8'))
            exit()

    return bytestring
        


def main():
    flag = "RITSEC{0n1On_L4y3R}"
    layer = 32 
    
    """
    # Creating the challenge - Use this for debugging purpose only. 
    # THIS WILL CHANGE THE CHALLENGE FILE. BECAREFUL.  
    
    # Perform base16,32,64 encoding <layer> times and create the challenge file. 
    challenge = creation(bytes(flag,'utf-8'), layer)

    print("\n[+] Onion layer encoded flag have been created\n")
    print("======================================================")
    
    fd = open("onionlayerencoding.txt", "w")
    fd.write(challenge.decode('utf-8'))
    """ 

    # ============================================================

    
    # Solving the challenge 
    
    # Set layer to 150, based on the challenge description 
    layer = 150

    print ("[+] Challenge Solver for Onion Layer Encoding") 

    fd = open("onionlayerencoding.txt", "r")

    for line in fd:
        print("[+] Opening challenge file")
        byteline = str.encode(line)

        print("[+] Solving Challenge... This will take some time...\n")
        solved = solution(byteline, layer)

    print("\n[+] Onion layer encoding SOLVED \n")
    print("======================================================\n")
    print(solved.decode('utf-8'))
    

if __name__ == '__main__':
    main()
