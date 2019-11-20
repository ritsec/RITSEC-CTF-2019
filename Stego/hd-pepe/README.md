HINT: Pepe is alpha tier
1. The given image is a 4k png
2. When looking at EXIF info, the message field points user to github.com/cyberme69420/hdpepe
3. Inside that repository is an "encoder.py" file, which encodes files into images.
4. Inside the encoder file:
    - the pixels of the image are looped through
    - a b64 string of the file to encode is generated
    - `alpha` values of each pixel are adjusted, based on the ord of the char in the b64 string

5. Users must create their own decoder:
    - iterate throught the pixels of the image
    - each alpha value represents "255 - ord(x)," where x is a char
    - put all the chars together, it's a b64 representation of a "flag.txt" file
    - write that b64 to disk, open it, contains the flag

Note: this talk might be useful: https://www.youtube.com/watch?v=iKUQROblTxo&t=613s
Note: the `examiner.py` will show the RGBA values (red, green, blue, alpha) for the image

flag: RITSEC{M3M3S_CAN_B3_M4LICIOUS}