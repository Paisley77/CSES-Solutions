#include <iostream>
#include <set>
#include <vector>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<int> colors(n + 1);
    for (int i = 0; i < n; ++i)
        std::cin >> colors[i + 1];

    std::vector<std::vector<int>> g(n + 1);
    int a, b;
    for (int i = 0; i < (n - 1); ++i)
    {
        std::cin >> a >> b;
        g[a].push_back(b);
        g[b].push_back(a);
    }

    std::vector<int> par(n + 1);
    std::vector<int> order;
    order.reserve(n);
    par[1] = 0;

    {
        std::vector<int> stk{1};
        stk.reserve(n);

        std::vector<char> vst(n + 1, 0);
        vst[1] = 1;
        while (!stk.empty())
        {
            int v = stk.back();
            stk.pop_back();
            order.push_back(v);
            for (int u : g[v])
            {
                if (!vst[u])
                {
                    vst[u] = 1;
                    par[u] = v;
                    stk.push_back(u);
                }
            }
        }
    }

    std::vector<std::set<int>> s(n + 1);
    std::vector<int> ans(n + 1);

    for (int i = n - 1; i >= 0; --i)
    {
        int v = order[i];
        int best = -1;
        for (int u : g[v])
        {
            if (u == par[v])
                continue;
            if (best == -1 || s[u].size() > s[best].size())
            {
                best = u;
            }
        }
        if (best != -1)
            s[v].swap(s[best]);
        for (int u : g[v])
        {
            if (u == par[v] || u == best)
                continue;
            for (int c : s[u])
                s[v].insert(c);
            std::set<int>().swap(s[u]);
        }
        s[v].insert(colors[v]);
        ans[v] = static_cast<int>(s[v].size());
    }

    for (int i = 1; i <= n; ++i)
    {
        std::cout << ans[i] << " \n"[i == n];
    }

    return 0;
}