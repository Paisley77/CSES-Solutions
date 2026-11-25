#include <iostream>
#include <vector>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<std::pair<int, int>>> graph(n + 1);
    for (int i = 0; i < m; ++i)
    {
        int u, v;
        std::cin >> u >> v;
        graph[u].push_back({v, i});
        graph[v].push_back({u, i});
    }
    std::vector<int> parent(n + 1, 0);
    std::vector<int> parentEdge(n + 1, 0);
    std::vector<int> tin(n + 1, 0);
    std::vector<int> next(n + 1, 0);
    std::vector<int> parity(n + 1, 0);
    std::vector<std::pair<int, int>> orn(m);
    std::vector<int> stk;
    stk.reserve(n);
    int timer = 0;
    for (int root = 1; root <= n; ++root)
    {
        if (tin[root] > 0)
            continue;
        tin[root] = ++timer;
        stk.push_back(root);
        while (!stk.empty())
        {
            int u = stk.back();
            if ((int)graph[u].size() == next[u])
            {
                stk.pop_back();
                if (parent[u] != 0)
                {
                    int p = parent[u];
                    int id = parentEdge[u];
                    if (parity[u] == 0)
                    {
                        orn[id] = {p, u};
                        parity[p] = 1 ^ parity[p];
                    }
                    else
                    {
                        orn[id] = {u, p};
                        parity[u] = 0;
                    }
                }
                else
                {
                    if (parity[u] == 1)
                    {
                        std::cout << "IMPOSSIBLE\n";
                        return 0;
                    }
                }
                continue;
            }
            auto [v, id] = graph[u][next[u]++];
            if (id == parentEdge[u])
                continue;
            if (tin[v] == 0)
            {
                parent[v] = u;
                parentEdge[v] = id;
                tin[v] = ++timer;
                stk.push_back(v);
            }
            else if (tin[v] < tin[u])
            {
                orn[id] = {u, v};
                parity[u] ^= 1;
            }
        }
    }
    for (auto [u, v] : orn)
    {
        std::cout << u << " " << v << '\n';
    }
    return 0;
}