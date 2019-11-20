

def sxor(s1,s2):    
    return ''.join(chr(ord(a) ^ ord(b)) for a,b in zip(s1,s2))


key = "REEvurs"

inp = bytes.fromhex('000c1125303108611d033f392d271a761a324126472f').decode()

out = ""
for i in range(len(inp)):
    out += sxor(inp[i], key[i%len(key)])

print(out)
