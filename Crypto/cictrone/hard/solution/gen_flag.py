from ecdsa import SigningKey, VerifyingKey
from ecdsa.util import sigdecode_string
from ecdsa.numbertheory import inverse_mod
from hashlib import sha256


priv_key = SigningKey.from_pem(open("private.pem").read())
pub_key = VerifyingKey.from_pem(open("public.pem").read())
print("=======================================================================================================")
print("Public Key")
print("\tCurve:\t\t\t", pub_key.curve.name)
print("\tGenerator X:\t\t", pub_key.pubkey.generator._Point__x)
print("\tGenerator Y:\t\t", pub_key.pubkey.generator._Point__y)
print("\tGenerator Order:\t", pub_key.pubkey.generator._Point__order)
print("\tPoint X:\t\t", pub_key.pubkey.point._Point__x)
print("\tPoint Y:\t\t", pub_key.pubkey.point._Point__y)
print("Private Key")
print("\tSecret Exponent:\t", priv_key.privkey.secret_multiplier)
print("=======================================================================================================")
print("=======================================================================================================")
msg1 = "We are not uncertain with the vote.".encode('utf-8')
msg2 = "We are uncertain with the vote.".encode('utf-8')
print("Hash 1:\t\t\t\t", int.from_bytes(sha256(msg1).digest(), byteorder='big'))
print("Hash 2:\t\t\t\t", int.from_bytes(sha256(msg2).digest(), byteorder='big'))
k = 7800010500099000107000320001190009700011500032000104000101000114000101
signature1 = priv_key.sign(msg1, k=k, hashfunc=sha256)
signature2 = priv_key.sign(msg2, k=k+1, hashfunc=sha256)
human_sign_1 = sigdecode_string(signature1, pub_key.pubkey.order)
human_sign_2 = sigdecode_string(signature2, pub_key.pubkey.order)
r1 = human_sign_1[0]
r2 = human_sign_2[0]
print("=======================================================================================================")
print("=======================================================================================================")
print("Signature 1")
print("\tR:\t\t\t", human_sign_1[0])
print("\tS:\t\t\t", human_sign_1[1])
print("Signature 2")
print("\tR:\t\t\t", human_sign_2[0])
print("\tS:\t\t\t", human_sign_2[1])
print("=======================================================================================================")
print("=======================================================================================================")
print("Signature Verified 1:\t\t", pub_key.verify(signature1, msg1, hashfunc=sha256))
print("Signature Verified 2:\t\t", pub_key.verify(signature2, msg2, hashfunc=sha256))
print("=======================================================================================================")
print("=======================================================================================================")
print("=======================================================================================================")
print("=======================================================================================================")
print("=======================================================================================================")
print("Finding Flag - Info")
order = pub_key.pubkey.generator._Point__order
x1    = int.from_bytes(sha256(msg1).digest(), byteorder='big')
x2    = int.from_bytes(sha256(msg2).digest(), byteorder='big')
s1    = human_sign_1[1]
s2    = human_sign_2[1]

print("\tGenerator Order:\t", order)
print("\tX_1:\t\t\t", x1)
print("\tX_2:\t\t\t", x2)
print("\tS_1:\t\t\t", s1)
print("\tS_2:\t\t\t", s2)
print("\tR_1:\t\t\t", r1)
print("\tR_2:\t\t\t ???")
print("=======================================================================================================")
print("=======================================================================================================")
print("Finding Flag - Calc R2")
print("\tFind v for u such that (u,v) is in G.")
print("\tSince k is just incremented then we add A to (u,v) in G.")
print("\tFind R_2 to be the x value of A+(u,v) =\t", r2)
print("=======================================================================================================")
print("=======================================================================================================")
print("Finding Flag - Calc Private Key")

top = (s2*x1 - s1*x2 + s1*s2) % order
print("\tTop = S_2*X_1 - S_1*X_2 + S_1*S_2 =\t", top)
bottom_inv = (r2*s1 - r1*s2) % order
print("\tBottom_Inv = R_2*S_1 - R_1*S_2 =\t", bottom_inv)
bottom = inverse_mod(bottom_inv, order)
print("\tBottom = Bottom_t Inverse in Z_order =\t", bottom)
priv_key_calc = (top * bottom) % order
print("\tCalulated Flag:\t\t\t", priv_key_calc)
print("\tFlag Correct:\t\t\t\t", priv_key_calc==priv_key.privkey.secret_multiplier)
print("=======================================================================================================")
