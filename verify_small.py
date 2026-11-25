"""Independent checks for A134320, A110194 and A103850 (standard library only)."""
from itertools import permutations
from math import isqrt

# A134320: brute force to 10^6 (the proof bounds every term by 210)
LIM = 10**6
divs = [[] for _ in range(LIM + 1)]
for d in range(1, LIM + 1):
    for m in range(d, LIM + 1, d):
        divs[m].append(d)
hits = []
for N in range(1, LIM + 1):
    D = set(divs[N])
    c = sum(1 for d in D if d - 1 in D or d + 1 in D)
    if 2 * c > len(D):
        hits.append(N)
print('A134320 terms <= 10^6:', hits)
assert hits == [2, 4, 6, 12, 20, 30, 42, 90]
# the halving lemma: for N = m(m+1), m >= 15 the divisor u (half the even factor) is isolated
for m in range(15, 5000):
    N = m * (m + 1); u = (m if m % 2 == 0 else m + 1) // 2
    assert N % u == 0 and N % (u - 1) and N % (u + 1), m

# A110194: definition vs the rule "4 if x+1 is prime, else 1"
def isprime(n):
    if n < 2: return False
    if n % 2 == 0: return n == 2
    r = isqrt(n); f = 3
    while f <= r:
        if n % f == 0: return False
        f += 2
    return True
x, a = 1, [1]                                 # OEIS a(1) = 1 is the starting term 1 of A110095
while len(a) < 3000:
    d = 1
    while isprime(d) or isprime(x + d): d += 1
    if len(a) >= 3: assert d == (4 if isprime(x + 1) else 1)
    a.append(d); x += d
oeis = [int(t) for t in "1,8,1,4,1,1,4,1,1,4,1,1,4,1,1,1,1,4,4,1,1,4,1,1,4,1,1,4,1,1,1,1,4,4,1,1,1,1,4,4,1,1,4,1,1,1,1,4,4,1,1,4,1,1,4,1,1,1,1,1,1,1,1,1,1,4,4,1,1,4,1,1,1,1,1,1,1,1,4,1,1,1,1,4,1,1,4,4,1,1,4,1,1,4,1".split(',')]
assert a[:len(oeis)] == oeis
s = ''.join(map(str, a))
i = s.find('11111')
print('A110194: first run of five 1s starts at a(%d); a(56..67) =' % (i + 1), a[55:67])
assert '444' not in s
runs = [len(r) for r in s.split('4') if r]
print('A110194: longest run of 1s among the first 3000 terms:', max(runs))

# A103850: least p + q over primes p, q that together use all ten digits
S = 10**5
sieve = bytearray([1]) * S; sieve[0] = sieve[1] = 0
for i in range(2, isqrt(S) + 1):
    if sieve[i]: sieve[i*i::i] = bytearray(len(sieve[i*i::i]))
P = [p for p in range(10**4, S) if sieve[p] and len(set(str(p))) == 5]
bymask = {}
for p in P:
    bymask.setdefault(frozenset(str(p)), []).append(p)
best, pairs = None, []
for m, lst in bymask.items():
    comp = frozenset('0123456789') - m
    if comp in bymask and min(lst) < min(bymask[comp]):
        for p in lst:
            for q in bymask[comp]:
                t = p + q
                if best is None or t < best: best, pairs = t, [(p, q)]
                elif t == best: pairs.append((min(p, q), max(p, q)))
pairs = sorted(set(pairs))
print('A103850: minimum', best, 'pairs', pairs)
assert best == 34146 and pairs == [(10457, 23689), (10459, 23687), (10687, 23459)]
assert all(isprime(v) for pq in pairs for v in pq)
print('ALL CHECKS PASSED')
