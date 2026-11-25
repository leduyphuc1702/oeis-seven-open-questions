// OEIS A336285: a(0)=0; a(n) = least unused positive integer such that a(n-1)+a(n) has
// distinct decimal digits.  Every term is below 9876543210 (the largest digit-distinct number),
// so the used set fits in a bitset and the greedy run is exhaustive: it stops exactly when no
// unused x with a(n-1)+x digit-distinct exists.
//
// For previous term p: x = least unused >= lo, T = least digit-distinct >= p+x, y = T-p.
// If y is unused it is the answer (no z in [x,y) has p+z digit-distinct); else lo = y+1.
// The search starts at the least unused positive integer mu, which never decreases.
// Every later term is >= mu, so once p + mu > 987654321 every admissible sum is a 10-digit
// digit-distinct number, i.e. uses all ten digits and is divisible by 9.  Then x = -p (mod 9),
// and the search may start at the least unused x in that residue class (muc[c]).
//
// stdout: "n a(n)" for n <= BFILE_N; stderr: progress, the final terms and a checksum.
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>
using namespace std;
typedef uint64_t u64;

static const u64 MAXV = 9876543210ULL;
static const u64 BFILE_N = 10000;
static vector<u64> V;
static vector<u64> used;  // bit v set: v already used
static vector<u64> full;  // bit w set: used word w is all ones

static void gen(u64 v, int len, int target, int mask) {
    if (len == target) { V.push_back(v); return; }
    for (int d = 0; d < 10; d++) {
        if ((len == 0 && d == 0) || (mask >> d & 1)) continue;
        gen(v * 10 + d, len + 1, target, mask | 1 << d);
    }
}
static inline bool isused(u64 v) { return used[v >> 6] >> (v & 63) & 1; }
static inline void setused(u64 v) {
    u64 w = v >> 6;
    used[w] |= 1ULL << (v & 63);
    if (used[w] == ~0ULL) full[w >> 6] |= 1ULL << (w & 63);
}
// least unused v >= lo (lo <= MAXV + 1 guaranteed to have an unused value since the bitset has slack)
static u64 nextunused(u64 lo) {
    u64 w = lo >> 6;
    u64 m = ~used[w] & (~0ULL << (lo & 63));
    if (m) return (w << 6) | __builtin_ctzll(m);
    w++;
    for (;;) {
        u64 s = w >> 6;
        u64 fm = ~full[s] & (~0ULL << (w & 63));
        if (fm) { w = (s << 6) | __builtin_ctzll(fm); break; }
        w = (s + 1) << 6;
    }
    return (w << 6) | __builtin_ctzll(~used[w]);
}

int main() {
    for (int L = 1; L <= 10; L++) gen(0, 0, L, 0);
    const size_t N = V.size();
    u64 words = ((MAXV + 1) >> 6) + 2;
    used.assign(words, 0);
    full.assign((words >> 6) + 2, 0);
    setused(0);
    u64 p = 0, n = 0, hops = 0, chk = 1469598103934665603ULL, mx = 0, mu = 1, lasthops = 0;
    u64 muc[9];
    for (int c = 0; c < 9; c++) muc[c] = c ? c : 9;
    u64 tail[20]; u64 tn = 0;
    printf("0 0\n");
    for (;;) {
        u64 lo = mu, ans = 0, h0 = hops;  // mu = least unused positive integer
        if (p + mu > 987654321ULL) { u64 c = (9 - p % 9) % 9; if (muc[c] > lo) lo = muc[c]; }
        for (;;) {
            hops++;
            u64 x = nextunused(lo);
            if (x > MAXV) break;
            size_t j = lower_bound(V.begin(), V.end(), p + x) - V.begin();
            if (j == N) break;
            u64 y = V[j] - p;
            if (!isused(y)) { ans = y; break; }
            lo = y + 1;
        }
        if (!ans) { lasthops = hops - h0; break; }
        setused(ans);
        if (ans == mu) mu = nextunused(mu);
        { u64 c = ans % 9; while (isused(muc[c])) muc[c] += 9; }
        p = ans; n++;
        if (ans > mx) mx = ans;
        chk = (chk ^ ans) * 1099511628211ULL;
        tail[tn % 20] = ans; tn++;
        if (n <= BFILE_N) printf("%llu %llu\n", (unsigned long long)n, (unsigned long long)ans);
        if ((n & ((1ULL << 24) - 1)) == 0) {
            fprintf(stderr, "n=%llu a=%llu max=%llu hops=%llu\n", (unsigned long long)n, (unsigned long long)ans, (unsigned long long)mx, (unsigned long long)hops);
            fflush(stderr);
        }
    }
    fprintf(stderr, "DONE last n=%llu a(n)=%llu max=%llu hops=%llu fnv=%016llx final_search_hops=%llu least_unused=%llu\n", (unsigned long long)n, (unsigned long long)p, (unsigned long long)mx, (unsigned long long)hops, (unsigned long long)chk, (unsigned long long)lasthops, (unsigned long long)mu);
    fprintf(stderr, "last terms:");
    u64 start = tn >= 20 ? tn % 20 : 0, cnt = tn < 20 ? tn : 20;
    for (u64 k = 0; k < cnt; k++) fprintf(stderr, " %llu", (unsigned long long)tail[(start + k) % 20]);
    fprintf(stderr, "\n");
    return 0;
}
