"""Independent greedy computation of OEIS A365197, written from the OEIS definition.

At step n >= 2 take the least unused positive integer x with distinct digits such that
x shares exactly d(n-1) distinct digits with a(n-1) and has at least d(n) digits, where
d(j) is the j-th digit of the concatenation a(1)a(2)...  Candidates are scanned by length
and, inside a length, in increasing order, so the first hit is the least admissible x.
"""
from itertools import permutations

def distinct_digit_numbers(length):
    for p in permutations('0123456789', length):
        if p[0] != '0':
            yield int(''.join(p))   # permutations of a sorted alphabet come out in increasing order

a = [1]
used = {1}
s = '1'
while True:
    n = len(a) + 1
    need_common = int(s[n - 2])
    # d(n) is undefined only at n = 2; the OEIS Mathematica program then imposes no length condition
    need_len = int(s[n - 1]) if len(s) >= n else 1
    assert len(s) >= n or n == 2
    prev = set(str(a[-1]))
    nxt = None
    for L in range(max(need_len, 1), 11):
        for x in distinct_digit_numbers(L):
            if x not in used and len(prev & set(str(x))) == need_common:
                nxt = x
                break
        if nxt is not None:
            break
    if nxt is None:
        break
    a.append(nxt); used.add(nxt); s += str(nxt)
print(len(a), a[-1])
print(','.join(map(str, a)))
