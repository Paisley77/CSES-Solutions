#include <iostream>
#include <vector>
#include <algorithm>

typedef long long ll;

const int MOD = 1e9 + 7;

std::vector<ll> solve_color(std::vector<int> &lengths, int k)
{
    std::sort(lengths.begin(), lengths.end());
    std::vector<ll> dp(k + 1, 0);
    dp[0] = 1;
    for (int len : lengths)
    {
        for (int j = k; j > 0; --j)
        {
            int choices = len - j + 1;
            if (choices > 0)
                dp[j] = (dp[j] + dp[j - 1] * choices % MOD) % MOD;
        }
    }
    return dp;
}

int main()
{
    int n, k;
    std::cin >> n >> k;
    std::vector<int> diag[2];
    for (int s = 0; s <= 2 * n - 2; ++s)
    {
        int L = std::min(s + 1, 2 * n - s - 1);
        diag[s & 1].push_back(L);
    }
    std::vector<ll> white = solve_color(diag[0], k);
    std::vector<ll> black = solve_color(diag[1], k);
    ll ans = 0;
    for (int i = 0; i <= k; ++i)
        ans = (ans + black[i] * white[k - i] % MOD) % MOD;
    std::cout << ans << '\n';
    return 0;
}