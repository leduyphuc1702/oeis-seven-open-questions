"""Independent computation of OEIS A364274 (greedy, distinct terms, cumulative sums with distinct digits).

With current sum Q and used set U, the next term is the least unused a with Q + a digit-distinct,
i.e. the least digit-distinct T > Q with T - Q not in U.  Scanning all digit-distinct T in
increasing order therefore reproduces the greedy rule, and when the list is exhausted no term remains.
Every digit-distinct number is at most 9876543210, so the scan is exhaustive.
"""
from itertools import permutations

def V():
    for L in range(1, 11):
        for p in permutations('0123456789', L):
            if p[0] != '0':
                yield int(''.join(p))

Q, used, terms, last = 0, set(), [], None
for T in V():
    a = T - Q
    if a <= 0 or a in used:
        continue
    used.add(a); terms.append((a, T)); Q = T
n = len(terms)
print(n, terms[-1])
with open('b364274_check.txt', 'w') as f:
    for i, (a, T) in enumerate(terms, 1):
        f.write(f'{i} {a} {T}\n')
