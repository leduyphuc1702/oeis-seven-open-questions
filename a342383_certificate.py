"""Termination certificate for OEIS A342383 from the computed b-file.

Reads the full list n a(n) (n = 0..last), checks the defining conditions term by term, and lists
every digit-distinct x for which a(last) + x is digit-distinct, with the index at which x was used.
If all of them are used, no further term exists.  Minimality of each term is checked by the two
C++ searches, not here.
"""
import sys
from itertools import permutations

path = sys.argv[1] if len(sys.argv) > 1 else 'b342383_full.txt'
a = [int(l.split()[1]) for l in open(path)]
ok = lambda v: len(set(str(v))) == len(str(v))
idx = {}
for n, v in enumerate(a):
    assert v not in idx
    idx[v] = n
    if n:
        assert v > 0 and ok(v) and ok(a[n - 1] + v), n
p = a[-1]
cands = []
for L in range(1, 11):
    for t in permutations('0123456789', L):
        if t[0] != '0':
            T = int(''.join(t))
            if T > p and ok(T - p):
                cands.append(T - p)
cands.sort()
unused = [x for x in cands if x not in idx]
print(f'last index {len(a) - 1}, a(last) = {p}')
print(f'{len(cands)} digit-distinct x with a(last)+x digit-distinct; unused among them: {len(unused)}')
print('smallest such x with index of use:', [(x, idx.get(x)) for x in cands[:8]])
assert not unused
print('CERTIFICATE OK: every admissible continuation is already used')
