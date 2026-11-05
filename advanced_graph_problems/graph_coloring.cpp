#include <iostream>
#include <vector>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, m;
    std::cin >> n >> m;
    int N = 1 << n;
    std::vector<int> adj(n, 0);
    for (int i = 0; i < m; ++i)
    {
        int a, b;
        std::cin >> a >> b;
        --a;
        --b;
        adj[a] |= (1 << b);
        adj[b] |= (1 << a);
    }
    std::vector<char> independent(N);
    independent[0] = 1;
    for (int mask = 1; mask < N; ++mask)
    {
        int v = __builtin_ctz(mask);
        int rest = mask & (mask - 1);
        independent[mask] = independent[rest] && !(adj[v] & rest);
    }
    std::vector<int> dp(N, n + 1);
    std::vector<int> choice(N, 0);
    dp[0] = 0;
    for (int mask = 1; mask < N; ++mask)
    {
        if (independent[mask])
        {
            dp[mask] = 1;
            choice[mask] = mask;
            continue;
        }
        int bit = mask & (-mask);
        int rest = mask ^ bit;
        for (int sub = rest;; sub = (sub - 1) & rest)
        {
            int group = sub | bit;
            if (independent[group])
            {
                int cand = dp[mask ^ group] + 1;
                if (cand < dp[mask])
                {
                    dp[mask] = cand;
                    choice[mask] = group;
                }
            }
            if (sub == 0)
                break;
        }
    }
    std::vector<int> color(n);
    int mask = N - 1;
    int currentColor = 1;
    while (mask)
    {
        int group = choice[mask];
        for (int bit = group; bit; bit &= (bit - 1))
        {
            color[__builtin_ctz(bit)] = currentColor;
        }
        mask ^= group;
        currentColor++;
    }
    std::cout << dp[N - 1] << '\n';
    for (int &c : color)
        std::cout << c << ' ';
    return 0;
}