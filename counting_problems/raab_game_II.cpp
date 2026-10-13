#include <iostream>
#include <vector>

typedef long long int64;

const int MOD = 1e9 + 7;

struct Test
{
    int n;
    int a;
    int b;
};

int64 mod_pow(int64 b, int64 p)
{
    int64 r = 1;
    while (p > 0)
    {
        if (p & 1)
            r = r * b % MOD;
        b = b * b % MOD;
        p >>= 1;
    }
    return r;
}

int main()
{
    // n! * C(n, n-a-b) * D[a+b][b]
    // D[n][k] = (n-1)D[n-2][k-1] + kD[n-1][k] + (n-k)D[n-1][k-1]
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int t;
    std::cin >> t;
    int MAXN = 0, MAXK = 0;
    std::vector<Test> tests(t);
    for (int idx = 0; idx < t; ++idx)
    {
        int n, a, b;
        std::cin >> n >> a >> b;
        tests[idx] = {n, a, b};
        MAXN = std::max(MAXN, n);
        MAXK = std::max(MAXK, a + b);
    }
    std::vector<std::vector<int>> queries(MAXK + 1);
    for (int idx = 0; idx < t; ++idx)
        queries[tests[idx].a + tests[idx].b].push_back(idx);
    std::vector<int64> fact(MAXN + 1), invFact(MAXN + 1);
    std::vector<std::vector<int64>> C(MAXN + 1, std::vector<int64>(MAXN + 1));
    fact[0] = 1;
    for (int i = 1; i <= MAXN; ++i)
        fact[i] = fact[i - 1] * i % MOD;
    invFact[MAXN] = mod_pow(fact[MAXN], MOD - 2);
    for (int i = MAXN - 1; i >= 0; --i)
        invFact[i] = invFact[i + 1] * (i + 1) % MOD;
    for (int n = 0; n <= MAXN; ++n)
        for (int r = 0; r <= n; ++r)
            C[n][r] = fact[n] * invFact[r] % MOD * invFact[n - r] % MOD;
    std::vector<int64> derangementValue(t);
    std::vector<int64> cur(MAXK + 1), prev(MAXK + 1), prev2(MAXK + 1);
    // D[0][0] = 1;
    prev[0] = 1;
    for (int idx : queries[0])
        derangementValue[idx] = 1;
    for (int n = 1; n <= MAXK; ++n)
    {
        for (int k = 0; k <= n; ++k)
        {
            int64 count = 0;
            if (k <= n - 1)
                count += (int64)(k * prev[k]);
            if (k >= 1)
                count += (int64)((n - k) * prev[k - 1]);
            if (k <= n - 1 && k >= 1)
                count += (int64)((n - 1) * prev2[k - 1]);
            cur[k] = (count % MOD);
        }
        for (int idx : queries[n])
        {
            int b = tests[idx].b;
            derangementValue[idx] = cur[b]; // D[a+b][b]
        }
        prev.swap(prev2);
        cur.swap(prev);
    }
    for (int idx = 0; idx < t; ++idx)
    {
        int n = tests[idx].n, a = tests[idx].a, b = tests[idx].b;
        if (a + b > n)
        {
            std::cout << 0 << '\n';
            continue;
        }
        int64 D = derangementValue[idx];
        int64 ans = ((fact[n] * C[n][n - a - b] % MOD) * D) % MOD;
        std::cout << ans << '\n';
    }
    return 0;
}