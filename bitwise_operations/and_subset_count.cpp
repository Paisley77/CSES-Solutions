#include <iostream>
#include <vector>

typedef long long int64;

const int MOD = 1e9 + 7;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    int M = 1;
    while (M <= n)
        M <<= 1;
    std::vector<int> cnt(M, 0);
    for (int i = 0; i < n; ++i)
    {
        int a;
        std::cin >> a;
        ++cnt[a];
    }
    /*
    At each bit b, cnt[mask] = #{elements that have the same bits as mask on b+1...BITS
    and are supermask of mask on 0..b}
    */
    for (int bit = 1; bit < M; bit <<= 1)
        for (int mask = 0; mask < M; ++mask)
            if (!(mask & bit))
                cnt[mask] += cnt[mask | bit];
    std::vector<int64> pow2(n + 1);
    pow2[0] = 1;
    for (int i = 1; i <= n; ++i)
        pow2[i] = 2 * pow2[i - 1] % MOD;
    std::vector<int64> F(M);
    /*
    Initialize F[k] = #{subsets whose AND are supermasks of k}
    */
    for (int k = 0; k < M; ++k)
        F[k] = (pow2[cnt[k]] - 1 + MOD) % MOD;
    /*
    At each bit b, F[mask] = #{subsets whose AND are supermasks of mask on b+1..BITS
    and equal mask on 0..b}
    */
    for (int bit = 1; bit < M; bit <<= 1)
        for (int mask = 0; mask < M; ++mask)
            if (!(mask & bit))
                F[mask] = (F[mask] - F[mask | bit] + MOD) % MOD;
    for (int k = 0; k <= n; ++k)
        std::cout << F[k] << " ";
    return 0;
}