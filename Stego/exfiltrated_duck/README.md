Forensics: Packet Capture and Stego Challenge, Exfiltrated Duck
===============================================================

Created by Security Risk Advisors for RITSEC Fall 2019

Note: The same two IP addresses are used for each transfer. This is simply
for convenience.

Packet capture 1
================

This packet capture includes two large chunks of data being sent across the
network. A single UDP packet seperates each chunk.

Save each chunk to a file and cat them together to form a single file.

This produces another packet capture that needs to be analyzed.


Packet capture 2
================

This packet capture includes three chunks of data separated by a single UDP
packet.

Save each chunk of data to a file then cat them together in the order they
were transmitted.

$ cat f1 > data
$ cat f2 >> data
$ cat f3 >> data

When combined a single file with base64 will be created, data.

Decode the file and create a new file.

$ base64 -d data > data-decoded

Determine what this new file is by running file.

$ file data-decoded
data-decoded: PNG image data, 2800 x 1867, 8-bit/color RGBA, non-interlaced


Stego
=====

Running binwalk reveals something interesting..

$ binwalk data-decoded.png 

DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
0             0x0             PNG image, 2800 x 1867, 8-bit/color RGBA, non-interlaced
64            0x40            Zlib compressed data, best compression
12558         0x310E          Zlib compressed data, best compression
10268434      0x9CAF12        Zip archive data, at least v2.0 to extract, compressed size: 81163, uncompressed size: 81630, name: dai2.jpg
10349741      0x9DECAD        End of Zip archive, footer length: 22

Running unzip will extract another image.

$ unzip data-decoded.png 
Archive:  data-decoded.png
warning [data-decoded.png]:  10268434 extra bytes at beginning or within zipfile
  (attempting to process anyway)
  inflating: dai2.jpg

Running binwalk on dai2.jpg reveals more interesting things.

$ binwalk dai2.jpg 

DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
0             0x0             JPEG image data, JFIF standard 1.01
30            0x1E            TIFF image data, little-endian offset of first image directory: 8
436           0x1B4           JPEG image data, JFIF standard 1.01
81417         0x13E09         Zip archive data, at least v1.0 to extract, compressed size: 49, uncompressed size: 49, name: garbage
81608         0x13EC8         End of Zip archive, footer length: 22

Running unzip will extract something useful.

$ unzip dai2.jpg
Archive:  dai2.jpg
warning [dai2.jpg]:  81417 extra bytes at beginning or within zipfile
  (attempting to process anyway)
 extracting: garbage

 $ cat garbage 
952bpNXY25WS51mcBt2Y1RUZoR1bUVWbvNGbld1eDV0UUlkU

Moar base64!

The base64 needs to be reversed before being decoded.

$ python3
>>> import base64
>>> s=b'952bpNXY25WS51mcBt2Y1RUZoR1bUVWbvNGbld1eDV0UUlkU'
>>> r=s[::-1]
>>> r
b'UklUU0VDe1dlbGNvbWVUb1RoZUR1Y2tBcm15SW52YXNpb259'
>>> base64.b64decode(r)
b'RITSEC{WelcomeToTheDuckArmyInvasion}'


