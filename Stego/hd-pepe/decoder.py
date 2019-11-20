from PIL import Image
import base64
import sys

def aValsToString(a_arr):
    print("[+] Converting A values...")
    bigStr = ""
    for a in a_arr:
        val = 255 - a
        c = chr(val)
        bigStr += c
    return bigStr

def getAVals(fname):
    aVals = []
    im = Image.open(fname) # Can be many different formats.
    pix = im.load()
    x_size, y_size = im.size  # Get the width and hight of the image for iterating over
    print("[+] Reading A values...")
    for x in range(x_size):
        for y in range(y_size):
            r, g, b, a = pix[x, y]
            if a == 255:
                return aVals
            aVals.append(a)

def writeOutFile(dataStr, fname):
    print("[+] Writing file...")
    data = base64.b64decode(dataStr)
    with open(fname, 'wb') as f:
        f.write(data)

def main():
    try:        
        inpImage = sys.argv[1]
        outFile = sys.argv[2]
    except:
        print("USAGE: python3 decoder.py <input_image> <output_file>")
        return
    print("[+] Reading file...")
    aVals = getAVals(inpImage)
    outStr = aValsToString(aVals)
    l = open("bigstr.txt", 'w+')
    l.write(outStr)
    writeOutFile(outStr, outFile)

main()