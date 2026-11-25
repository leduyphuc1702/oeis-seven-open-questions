// OEIS A336285, second implementation (different search loop from a336285.cpp).
// a(0)=0; a(n) = least unused positive integer x such that a(n-1)+x has distinct digits.
// For previous term p, the admissible x are exactly T-p for digit-distinct T > p; scanning
// the sorted list of digit-distinct numbers T upward from p+mu (mu = least unused positive
// integer, so every smaller x is used) and taking the first T with T-p unused gives the least
// admissible x.  The used set is a plain bitset (no summary levels); mu is advanced by a
// linear scan.  All later terms are >= mu, so when p + mu > 987654321 every admissible sum has
// ten distinct digits, hence is divisible by 9, and x = -p (mod 9): the scan may then start at
// p + mc[-p mod 9], where mc[c] is the least unused positive integer congruent to c mod 9.  Prints the same summary line (count, last term, FNV-1a checksum) as a336285.cpp.
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <vector>
using namespace std;
typedef uint64_t u64;
static const u64 MAXV = 9876543210ULL;
static vector<u64> V, used;

static void gen(u64 v, int len, int target, int mask) {
    if (len == target) { V.push_back(v); return; }
    for (int d = 0; d < 10; d++) {
        if ((len == 0 && d == 0) || (mask >> d & 1)) continue;
        gen(v * 10 + d, len + 1, target, mask | 1 << d);
    }
}
static inline bool isused(u64 v) { return used[v >> 6] >> (v & 63) & 1; }

int main() {
    for (int L = 1; L <= 10; L++) gen(0, 0, L, 0);
    const size_t N = V.size();
    used.assign(((MAXV + 1) >> 6) + 2, 0);
    used[0] = 1;  // a(0) = 0
    u64 mc[9];
    for (int c = 0; c < 9; c++) mc[c] = c ? c : 9;
    u64 p = 0, n = 0, mu = 1, chk = 1469598103934665603ULL, mx = 0, scanned = 0, lastscan = 0;
    for (;;) {
        u64 start = mu;
        if (p + mu > 987654321ULL) { u64 c = (9 - p % 9) % 9; if (mc[c] > start) start = mc[c]; }
        size_t j = lower_bound(V.begin(), V.end(), p + start) - V.begin();
        u64 ans = 0, s0 = scanned;
        for (; j < N; j++) {
            scanned++;
            u64 x = V[j] - p;
            if (!isused(x)) { ans = x; break; }
        }
        if (!ans) { lastscan = scanned - s0; break; }
        used[ans >> 6] |= 1ULL << (ans & 63);
        while (isused(mu)) mu++;
        while (isused(mc[ans % 9])) mc[ans % 9] += 9;
        p = ans; n++;
        if (ans > mx) mx = ans;
        chk = (chk ^ ans) * 1099511628211ULL;
        if ((n & ((1ULL << 24) - 1)) == 0) { fprintf(stderr, "n=%llu a=%llu max=%llu\n", (unsigned long long)n, (unsigned long long)ans, (unsigned long long)mx); fflush(stderr); }
    }
    fprintf(stderr, "DONE last n=%llu a(n)=%llu max=%llu fnv=%016llx final_scan=%llu least_unused=%llu\n", (unsigned long long)n, (unsigned long long)p, (unsigned long long)mx, (unsigned long long)chk, (unsigned long long)lastscan, (unsigned long long)mu);
    return 0;
}
