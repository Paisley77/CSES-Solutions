#include <vector>
#include <iostream>

const long long MOD = 1e9 + 7;

long long power(long long base, long long exp)
{
    long long r = 1;
    base %= MOD;
    while (exp > 0)
    {
        if (exp & 1)
            r = r * base % MOD;
        base = base * base % MOD;
        exp >>= 1;
    }
    return r;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<int> p(n + 1);
    for (int i = 1; i <= n; ++i)
        std::cin >> p[i];

    std::vector<int> spf(n + 1, 0);
    for (int i = 2; i <= n; ++i)
    {
        if (spf[i] == 0)
        {
            for (int j = i; j <= n; j += i)
            {
                if (spf[j] == 0)
                    spf[j] = i;
            }
        }
    }

    std::vector<char> vst(n + 1, 0);
    std::vector<int> maxexp(n + 1, 0);

    for (int i = 1; i <= n; ++i)
    {
        if (vst[i])
            continue;
        int length = 0;
        for (int v = i; !vst[v]; v = p[v])
        {
            vst[v] = 1;
            ++length;
        }

        for (int L = length; L > 1;)
        {
            int q = spf[L], exp = 0;
            while (L % q == 0)
            {
                L /= q;
                ++exp;
            }
            if (exp > maxexp[q])
                maxexp[q] = exp;
        }
    }

    long long ans = 1;
    for (int q = 2; q <= n; ++q)
    {
        if (maxexp[q] > 0)
            ans = ans * power(q, maxexp[q]) % MOD;
    }

    std::cout << ans << '\n';
    return 0;
}