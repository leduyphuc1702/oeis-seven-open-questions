// OEIS A342383: a(0)=0; a(n) = least unused positive integer with distinct digits such that
// a(n-1)+a(n) also has distinct digits.  Exhaustive greedy over the 8877690 digit-distinct
// positive integers (all <= 9876543210), so the run ends exactly when no next term exists.
//
// For a fixed previous term p we need the least unused x in V with p+x in V.  A leapfrog
// between the unused part of V (successor structure over sorted indices) and V-p finds it:
// every skipped x either has p+x outside V or is used, and x only increases.
//
// Output: b-file lines "n a(n)" on stdout, summary on stderr.
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <vector>
using namespace std;
typedef uint64_t u64;

static vector<u64> V;
static vector<uint32_t> nxt;  // nxt[i] = least unused index >= i (path-compressed), N = sentinel

static uint32_t findu(uint32_t i) {
    uint32_t r = i;
    while (nxt[r] != r) r = nxt[r];
    while (nxt[i] != r) { uint32_t t = nxt[i]; nxt[i] = r; i = t; }
    return r;
}

static void gen(u64 v, int len, int target, int mask) {
    if (len == target) { V.push_back(v); return; }
    for (int d = 0; d < 10; d++) {
        if (len == 0 && d == 0) continue;
        if (mask >> d & 1) continue;
        gen(v * 10 + d, len + 1, target, mask | 1 << d);
    }
}

int main(int argc, char **argv) {
    for (int L = 1; L <= 10; L++) gen(0, 0, L, 0);
    const uint32_t N = V.size();
    for (uint32_t i = 1; i < N; i++) if (V[i] <= V[i - 1]) { fprintf(stderr, "not sorted\n"); return 1; }
    nxt.resize(N + 1);
    for (uint32_t i = 0; i <= N; i++) nxt[i] = i;
    u64 p = 0, n = 0, hops = 0;
    printf("0 0\n");
    for (;;) {
        uint32_t i = findu(0);
        bool found = false;
        while (i < N) {
            hops++;
            u64 x = V[i], T = p + x;
            uint32_t j = lower_bound(V.begin(), V.end(), T) - V.begin();
            if (j == N) break;
            if (V[j] == T) { found = true; break; }
            u64 x2 = V[j] - p;
            i = findu(lower_bound(V.begin(), V.end(), x2) - V.begin());
        }
        if (!found) break;
        u64 x = V[i];
        nxt[i] = i + 1;
        p = x; n++;
        printf("%llu %llu\n", (unsigned long long)n, (unsigned long long)x);
        if ((n & ((1 << 20) - 1)) == 0) fprintf(stderr, "n=%llu a=%llu hops=%llu\n", (unsigned long long)n, (unsigned long long)x, (unsigned long long)hops);
    }
    fprintf(stderr, "DONE last n=%llu a(n)=%llu hops=%llu |V|=%u\n", (unsigned long long)n, (unsigned long long)p, (unsigned long long)hops, N);
    return 0;
}
