#include <vector>
#include <iostream>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, x;
    std::cin >> n >> x;
    std::vector<int> w(n);
    for (auto &p : w)
        std::cin >> p;
    int nMasks = 1 << n;
    std::vector<std::pair<int, int>> dp(nMasks, {n + 1, 0}); // #{rides}, last weight
    dp[0] = {1, 0};
    for (int mask = 0; mask < nMasks; ++mask)
    {
        for (int i = 0; i < n; ++i)
        {
            if (((1 << i) & mask) == 0)
                continue;
            int prevMask = mask ^ (1 << i);
            auto candidate = dp[prevMask];
            if (w[i] + candidate.second <= x)
                candidate.second += w[i];
            else
            {
                candidate.first++;
                candidate.second = w[i];
            }
            dp[mask] = std::min(dp[mask], candidate);
        }
    }
    std::cout << dp[nMasks - 1].first << '\n';
    return 0;
}