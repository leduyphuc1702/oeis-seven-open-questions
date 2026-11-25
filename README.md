# Answers to seven open questions in the OEIS

Le Duy Phuc, 2026-11-25. Paper: `paper.pdf` (source `paper.tex`).

| Entry | Question in the entry | Answer |
|---|---|---|
| A364274 | last term? | 59388 terms, a(59388) = 250983 (final partial sum 9876501324) |
| A365197 | last term? | 75 terms, a(75) = 1023465 |
| A342383 | number of terms? | a(0..4157155), last term a(4157155) = 896075231 |
| A336285 | number of terms? | a(0..1481482307), last term a(1481482307) = 4938274219 |
| A134320 | complete? | yes, exactly 2, 4, 6, 12, 20, 30, 42, 90 |
| A110194 | only blocks 114 and 111144? | no: a(56..65) are ten 1's; exact rule and run lengths proved |
| A103850 | least p+q using all ten digits? | 34146: 10457+23689, 10459+23687, 10687+23459 (pair already published by Kurland 2022) |

## Files

| File | Content |
|---|---|
| `a364274.py` | scan of the 8877690 digit-distinct numbers; writes `b364274_check.txt` (n, a(n), partial sum) |
| `A364274_terminal.tsv` | the 117 digit-distinct sums above the final sum, each with T − Q and the index of that earlier term |
| `a365197.py` | greedy search; output `a365197.out` |
| `a342383.cpp`, `a342383_bits.cpp` | two independent programs for A342383 (identical output) |
| `a342383_certificate.py` | rechecks every term of the full list and proves no further term exists |
| `a336285.cpp`, `a336285_scan.cpp` | two independent programs for A336285 (same length, last term, largest term, FNV-1a checksum `2be9fa44ae3f7d9d`) |
| `literal_check.py` | literal greedy for the first 10000 terms of A342383 and A336285 |
| `verify_small.py` | A134320, A110194, A103850 |
| `b364274.txt`, `b365197.txt` | complete sequences |
| `b342383.txt`, `b336285.txt` | a(0..10000) |
| `b342383_full.txt.gz` | complete A342383, lines `n a(n)`; SHA-256 of the unpacked file `491a1f736cc7c97cba6a8f191d49333a322f4b970526aa1b030de5f1be75fdc8` |

## Reproduce

```sh
python3 a364274.py && python3 a365197.py > a365197.out && python3 verify_small.py
c++ -O2 -std=c++17 -o a342383 a342383.cpp && ./a342383 > b342383_full.txt   # about 8 minutes
python3 a342383_certificate.py b342383_full.txt
c++ -O2 -std=c++17 -o a336285_scan a336285_scan.cpp && ./a336285_scan > /dev/null   # about 2 minutes, needs 1.3 GB RAM
c++ -O2 -std=c++17 -o a336285 a336285.cpp && ./a336285 > b336285.txt             # about 9 minutes
python3 literal_check.py 10000
```

The A336285 programs print the summary line (`DONE last n=1481482307 a(n)=4938274219 max=4938274688 fnv=2be9fa44ae3f7d9d ...`) on standard error.

## Credits and AI disclosure

The questions are due to Eric Angelini (A364274, A365197), Rémy Sigrist (A336285), Scott R. Shannon (A342383), Leroy Quet and Hugo van der Sanden (A134320), Alexandre Wajnberg and Eric Angelini (A110194) and Zak Seidov (A103850). Drafts of the results for A134320, A110194, A103850, A364274 and A365197 were prepared with AI tools; every statement was re-derived and every computation redone with independently written programs. AI assistance was also used for code and text; the author checked everything.

License: CC BY 4.0.
