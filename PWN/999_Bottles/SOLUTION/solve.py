import glob
import os
from pwn import *
import r2pipe
import re
import json

def capture(file):
    r2 = r2pipe.open(file, flags=['-2'])
    r2.cmd('aaa')

    objectDiscovery = r2.cmd('pdj 1 @`pdf@main~movzx eax[0]`')
    jsonObject = json.loads(objectDiscovery)
    currentObject = jsonObject[0].get('disasm').rstrip().rstrip("]")
    data = re.findall(r"obj.*", currentObject)[0]
    addresses = r2.cmd('pdf@main~{}[0]'.format(data)).split()[-2]
    residualData = chr(int(json.loads(r2.cmd('pdj 1 @ {}'.format(addresses)))[0].get('disasm').split()[-1], 16))

    return residualData


if __name__ == '__main__':
    flag = list('X' * len(glob.glob('*.out')))

    for index, file in enumerate(sorted(glob.glob('*.out'))):
        if index:
            print('Enumerating: {}'.format(file), end=': ')

            flag[index] = capture(file)
            print(flag[index])

        if re.findall(r'RITSEC\{.*\}', ''.join(flag)):
            print(re.findall(r'RITSEC\{.*\}', ''.join(flag))[0])
            break
