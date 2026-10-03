#include <iostream>
#include <vector>
#include <string>

const int MOD = 1e9 + 7;
const int MAXN = 500;
long long fact[MAXN + 1], invFact[MAXN + 1], nCr[MAXN + 1][MAXN + 1];

long long power(long long b, long long e)
{
    int r = 1;
    while (e != 0)
    {
        if (e & 1)
            r = r * b % MOD;
        b = b * b % MOD;
        e >>= 1;
    }
    return r;
}

int modInv(long long b) { return power(b, MOD - 2); }

void precompute(int n)
{
    fact[0] = 1;
    for (int i = 1; i <= n; ++i)
        fact[i] = fact[i - 1] * i % MOD;
    invFact[n] = modInv(fact[n]);
    for (int i = n; i >= 1; --i)
        invFact[i - 1] = invFact[i] * i % MOD;
    for (int i = 0; i <= n; ++i)
        for (int j = 0; j <= i; ++j)
            nCr[i][j] = fact[i] * invFact[j] % MOD * invFact[i - j] % MOD;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    precompute(n);
    std::vector<int> rowA(n, -1), colA(n, -1), rowB(n, -1), colB(n, -1);
    int rE, cE, gA, gB, lA, lB;
    rE = cE = gA = gB = 0;
    lA = lB = n;
    for (int i = 0; i < n; ++i)
    {
        std::string s;
        std::cin >> s;
        for (int j = 0; j < n; ++j)
        {
            if (s[j] == 'A')
            {
                rowA[i] = j;
                colA[j] = i;
            }
            else if (s[j] == 'B')
            {
                rowB[i] = j;
                colB[j] = i;
            }
        }
    }

    for (int i = 0; i < n; ++i)
    {
        if (rowA[i] == -1 && rowB[i] == -1)
            rE++;
        if (colA[i] == -1 && colB[i] == -1)
            cE++;
        if (rowA[i] != -1 && rowB[i] == -1 && colB[rowA[i]] == -1)
            gA++;
        if (rowB[i] != -1 && rowA[i] == -1 && colA[rowB[i]] == -1)
            gB++;
        if (rowA[i] != -1)
            lA--;
        if (rowB[i] != -1)
            lB--;
    }

    long long ans = 0;
    for (int i = 0; i <= std::min(rE, cE); ++i)
    {
        long long O1 = nCr[rE][i] * nCr[cE][i] % MOD * fact[i] % MOD;
        long long O2 = 0;
        for (int j = 0; j <= gA; ++j)
        {
            long long S = nCr[gA][j] * fact[lB - i - j] % MOD;
            if (j % 2 == 1)
                O2 = (O2 - S + MOD) % MOD;
            else
                O2 = (O2 + S) % MOD;
        }
        long long O3 = 0;
        for (int k = 0; k <= gB; ++k)
        {
            long long S = nCr[gB][k] * fact[lA - i - k] % MOD;
            if (k % 2 == 1)
                O3 = (O3 - S + MOD) % MOD;
            else
                O3 = (O3 + S) % MOD;
        }
        long long F = O1 * O2 % MOD * O3 % MOD;
        if (i % 2 == 1)
            ans = (ans - F + MOD) % MOD;
        else
            ans = (ans + F) % MOD;
    }

    std::cout << ans;
    return 0;
}