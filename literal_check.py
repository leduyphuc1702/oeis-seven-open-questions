"""Literal greedy implementations of A342383 and A336285 (first N terms),
compared with the b-files produced by the C++ programs."""
import sys


def dd(x):
    s = str(x)
    return len(set(s)) == len(s)


def literal(n_terms, need_term_dd):
    seq = [0]
    used = set()
    mu = 1
    while len(seq) < n_terms:
        p = seq[-1]
        x = mu
        while x in used or (need_term_dd and not dd(x)) or not dd(p + x):
            x += 1
        seq.append(x)
        used.add(x)
        while mu in used:
            mu += 1
    return seq


def read_b(path, n):
    out = []
    with open(path) as f:
        for line in f:
            i, a = line.split()
            out.append(int(a))
            if len(out) == n:
                break
    return out


N = int(sys.argv[1]) if len(sys.argv) > 1 else 10000
for name, path, flag in [("A342383", "b342383_full.txt", True),
                         ("A336285", "b336285.txt", False)]:
    ref = literal(N, flag)
    got = read_b(path, N)
    assert got == ref, name
    print(name, "first", N, "terms match literal greedy")
print("LITERAL OK")
