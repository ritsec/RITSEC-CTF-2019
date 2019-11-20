from ctypes import (windll, wintypes, c_uint64, cast, POINTER, c_char,
                    LittleEndianStructure, byref, c_size_t, c_uint32)
import sys
import zlib


# types and flags
DELTA_FLAG_TYPE             = c_uint64
DELTA_FLAG_NONE             = 0x00000000
DELTA_APPLY_FLAG_ALLOW_PA19 = 0x00000001
DELTA_FILE_TYPE_RAW = 1
DELTA_FILE_TYPE_SET_RAW_ONLY = DELTA_FILE_TYPE_RAW


# structures
class DELTA_INPUT(LittleEndianStructure):
    _fields_ = [('lpStart', wintypes.LPVOID),
                ('uSize', c_size_t),
                ('Editable', wintypes.BOOL)]


class DELTA_OUTPUT(LittleEndianStructure):
    _fields_ = [('lpStart', POINTER(c_char)),
                ('uSize', c_size_t)]


# functions
CreateDeltaB = windll.msdelta.CreateDeltaB
CreateDeltaB.argtypes = [DELTA_FLAG_TYPE, DELTA_FLAG_TYPE, DELTA_FLAG_TYPE,
                         DELTA_INPUT, DELTA_INPUT, DELTA_INPUT, DELTA_INPUT,
                         DELTA_INPUT, wintypes.LPVOID, c_uint32,  # ALG_ID
                         POINTER(DELTA_OUTPUT)]
CreateDeltaB.rettype = wintypes.BOOL
DeltaFree = windll.msdelta.DeltaFree
DeltaFree.argtypes = [wintypes.LPVOID]
DeltaFree.rettype = wintypes.BOOL

unpatched = open(sys.argv[1], 'rb').read()
patched = open(sys.argv[2], 'rb').read()

src = DELTA_INPUT(cast(unpatched, wintypes.LPVOID), len(unpatched), False)
dst = DELTA_INPUT(cast(patched, wintypes.LPVOID), len(patched), False)
empty = DELTA_INPUT()
res = DELTA_OUTPUT()

status = CreateDeltaB(DELTA_FILE_TYPE_SET_RAW_ONLY, DELTA_FLAG_NONE,
                      DELTA_FLAG_NONE, src, dst, empty, empty, empty, None,
                      0x8003,
                      byref(res))
if status == 0:
    raise Exception("Patchy failed")
    sys.exit(1)

out = res.lpStart[:res.uSize]
out = zlib.crc32(out).to_bytes(4, 'little') + out
open('patch-tuesday', 'wb').write(out)
DeltaFree(res.lpStart)
