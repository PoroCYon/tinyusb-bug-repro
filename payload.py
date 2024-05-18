#!/usr/bin/env python3

import sys, os, struct

data = open(sys.argv[1],'rb').read()
_ptr = 0

def read(n):
    global _ptr
    rv = data[_ptr:_ptr+n]
    _ptr = _ptr + n
    return rv

hdr = read(6)
assert hdr == b'DC5LIB'
nbs = struct.unpack('<H', read(2))[0]

for i in range(nbs):
    name = read(12).decode('ascii').strip()
    boff, blen = struct.unpack('<II', read(8))

    boff = boff - 1

    print(name, hex(boff), hex(blen))

    open(name,'wb').write(data[boff:boff+blen])
