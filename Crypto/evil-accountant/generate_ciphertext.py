#!/usr/bin/env python3

from Crypto.Cipher import AES

import base64

FLAG = 'RITSEC{K0rr3LA710n_CAN_50M371m35_1MpLY_cau5A710N_24544110ad75a4}'
key = bytes([43, 126, 21, 22, 40, 174, 210, 166, 171, 247, 21, 136, 9, 207, 79, 60])

cipher = AES.new(key, AES.MODE_ECB)
msg = cipher.encrypt(FLAG)
print(base64.b64encode(msg).decode())
