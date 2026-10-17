#include <iostream>
#include <queue>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    std::vector<int> next(n);
    std::vector<int> indeg(n, 0);
    std::vector<std::vector<int>> rev(n);

    for (int i = 0; i < n; ++i)
    {
        std::cin >> next[i];
        next[i]--;
        indeg[next[i]]++;
        rev[next[i]].push_back(i);
    }

    // remove all non-cycle nodes
    std::queue<int> q;
    for (int i = 0; i < n; ++i)
        if (indeg[i] == 0)
            q.push(i);
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        indeg[next[u]]--;
        if (indeg[next[u]] == 0)
            q.push(next[u]);
    }

    // process cycle nodes
    std::vector<char> seen(n, 0);
    std::vector<int> ans(n);
    for (int i = 0; i < n; ++i)
    {
        if (indeg[i] == 0 || seen[i])
            continue;
        std::vector<int> cycle;
        int u = i;
        while (!seen[u])
        {
            seen[u] = 1;
            cycle.push_back(u);
            u = next[u];
        }
        int len = (int)cycle.size();
        for (int u : cycle)
        {
            ans[u] = len;
            q.push(u);
        }
    }

    // process non-cycle nodes
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (int v : rev[u])
        {
            // skip cycle nodes
            if (seen[v])
                continue;
            ans[v] = ans[u] + 1;
            q.push(v);
        }
    }

    for (auto &v : ans)
        std::cout << v << '\n';
    return 0;
}