// OEIS A342383, second implementation (independent data structures from a342383.cpp).
// a(0)=0; a(n) = least unused positive integer with distinct digits such that a(n-1)+a(n)
// also has distinct digits.  Membership in V (digit-distinct numbers) and in the unused part
// of V are bitsets over [0, 9876543210] with one summary level for "next set bit" queries.
// For previous term p: x = next unused member of V >= lo; if p+x is in V, x is the answer;
// otherwise lo = (next member of V >= p+x) - p.  This is exhaustive, so the run stops
// exactly when no further term exists.
//
// stdout: "n a(n)" for every term; stderr: progress and summary.
#include <cstdint>
#include <cstdio>
#include <vector>
using namespace std;
typedef uint64_t u64;
static const u64 MAXV = 9876543210ULL;

struct Bits {
    vector<u64> b, s;
    void init(u64 nbits) { b.assign((nbits >> 6) + 2, 0); s.assign((b.size() >> 6) + 2, 0); }
    bool get(u64 v) const { return b[v >> 6] >> (v & 63) & 1; }
    void set(u64 v) { b[v >> 6] |= 1ULL << (v & 63); s[v >> 12] |= 1ULL << ((v >> 6) & 63); }
    void clear(u64 v) {
        u64 w = v >> 6;
        b[w] &= ~(1ULL << (v & 63));
        if (!b[w]) s[w >> 6] &= ~(1ULL << (w & 63));
    }
    // least set bit >= y, or ~0 if none
    u64 next(u64 y) const {
        u64 w = y >> 6;
        if (w >= b.size()) return ~0ULL;
        u64 m = b[w] & (~0ULL << (y & 63));
        if (m) return (w << 6) | __builtin_ctzll(m);
        w++;
        u64 si = w >> 6;
        if (si >= s.size()) return ~0ULL;
        u64 sm = (w & 63) ? (s[si] & (~0ULL << (w & 63))) : s[si];
        while (!sm) { if (++si >= s.size()) return ~0ULL; sm = s[si]; }
        w = (si << 6) | __builtin_ctzll(sm);
        return (w << 6) | __builtin_ctzll(b[w]);
    }
};

static Bits Vb, Ub;
static u64 cnt = 0;
static void gen(u64 v, int len, int mask) {
    if (len) { Vb.set(v); Ub.set(v); cnt++; }
    if (len == 10) return;
    for (int d = 0; d < 10; d++) {
        if ((len == 0 && d == 0) || (mask >> d & 1)) continue;
        gen(v * 10 + d, len + 1, mask | 1 << d);
    }
}

int main() {
    Vb.init(MAXV + 64); Ub.init(MAXV + 64);
    gen(0, 0, 0);
    fprintf(stderr, "|V| = %llu\n", (unsigned long long)cnt);
    u64 p = 0, n = 0, hops = 0;
    printf("0 0\n");
    for (;;) {
        u64 lo = 1, ans = 0;
        for (;;) {
            hops++;
            u64 x = Ub.next(lo);
            if (x == ~0ULL || x > MAXV || p + x > MAXV) break;
            if (Vb.get(p + x)) { ans = x; break; }
            u64 T = Vb.next(p + x);
            if (T == ~0ULL || T > MAXV) break;
            lo = T - p;
        }
        if (!ans) break;
        Ub.clear(ans);
        p = ans; n++;
        printf("%llu %llu\n", (unsigned long long)n, (unsigned long long)ans);
        if ((n & ((1 << 18) - 1)) == 0) { fprintf(stderr, "n=%llu a=%llu hops=%llu\n", (unsigned long long)n, (unsigned long long)ans, (unsigned long long)hops); fflush(stderr); }
    }
    fprintf(stderr, "DONE last n=%llu a(n)=%llu hops=%llu\n", (unsigned long long)n, (unsigned long long)p, (unsigned long long)hops);
    return 0;
}
