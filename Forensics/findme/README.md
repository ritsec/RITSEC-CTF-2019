RITSEC 2019
Challenges created by Security Risk Advisors


Forensics, packet capture
=========================

Tools required:
- WireShark
- nc
- base64 decoder
- tar

The packet captures include the following requests and responses between
192.168.0.109 and 18.219.169.113. Participants need to analyse the various packets
and determine what it is they must do in order to progress through the
challenge. The various types of packets sent to various ports on 18.219.169.113
are meant to be clues. Some packets look like pings that fail while others
like look successful connections and/or SYN packets followed by SYN/ACK packets.

The participant is required to use netcat, nc, to connect to port 1337 and see
what happens.

$ nc 18.219.169.113 1337 

Running the nc command above returns the following.

H4sIAOKnx10AA+3OvQrCMADE8cx9ijxC0qbJKoiDq7qHaP0oSAltMonvbouLg+hURPj/lhvuhjtd
w1nMTI2sNVNqV6vXfDKl0FVttSsrp8ed1pVxQqq5j03ykEIvpcj73KX8Yfel/1Ob9W67Wt7iIcTB
q963yTdt0yV/WcR47O5F8euHAAAAAAAAAAAAAAAAAIB3HhZRz7sAKAAA

This is base64 that can be saved to a file and then decoded.

echo "H4sIAOKnx10AA+3OvQrCMADE8cx9ijxC0qbJKoiDq7qHaP0oSAltMonvbouLg+hURPj/lhvuhjtd
w1nMTI2sNVNqV6vXfDKl0FVttSsrp8ed1pVxQqq5j03ykEIvpcj73KX8Yfel/1Ob9W67Wt7iIcTB
q963yTdt0yV/WcR47O5F8euHAAAAAAAAAAAAAAAAAIB3HhZRz7sAKAAA" > file

$ base64 -d file > file-decoded

Determine the type of file the base64 decodes to.

$ file file-decoded 
file-decoded: gzip compressed data, last modified: Sun Nov 10 06:02:10 2019, from Unix, original size 10240

This file is intended to be uncompressed using tar, not gunzip. Use the v flag to see
what happens.

$ tar xvf file-decoded
flag

The final step is to display the contents of the file flag.

$ cat flag

RITSEC{pcaps_0r_it_didnt_h@ppen}


