#include <iostream>
#include <vector>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, m, q;
    std::cin >> n >> m >> q;
    int LOG = 1; // we need 2 ^ (LOG - 1) >= n - 1
    while (1 << (LOG - 1) < (n - 1))
        ++LOG;
    std::vector<std::vector<int>> graph(n + 1);
    for (int i = 0; i < m; ++i)
    {
        int a, b;
        std::cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }
    std::vector<int> parent(n + 1, 0);
    std::vector<int> tin(n + 1, 0);
    std::vector<int> tout(n + 1, 0);
    std::vector<int> low(n + 1, 0);
    std::vector<int> depth(n + 1, 0);
    std::vector<int> next(n + 1, 0);
    std::vector<std::vector<int>> up(n + 1, std::vector<int>(LOG, 1));
    std::vector<int> stk;
    stk.reserve(n);
    int timer = 1;
    tin[1] = low[1] = timer;
    stk.push_back(1);
    while (!stk.empty())
    {
        int u = stk.back();
        if (next[u] == (int)graph[u].size())
        {
            stk.pop_back();
            tout[u] = timer;
            if (parent[u] != 0)
            {
                int p = parent[u];
                low[p] = std::min(low[p], low[u]);
            }
            continue;
        }
        int v = graph[u][next[u]++];
        if (v == parent[u])
            continue;
        if (tin[v] == 0)
        {
            tin[v] = low[v] = ++timer;
            parent[v] = u;
            depth[v] = depth[u] + 1;
            up[v][0] = u;
            for (int j = 1; j < LOG; ++j)
                up[v][j] = up[up[v][j - 1]][j - 1];
            stk.push_back(v);
        }
        else if (tin[v] < tin[u])
            low[u] = std::min(low[u], tin[v]);
    }
    auto group = [&](int c, int x) -> int
    {
        /*
        pre: x and c are distinct vertices
        post: returns the component index which x belongs to after removing c
        */
        if (tin[x] < tin[c] || tin[x] > tout[c])
            return 0;
        int dist = depth[x] - depth[c] - 1;
        for (int j = LOG - 1; j >= 0; --j)
            if (dist & (1 << j))
                x = up[x][j];
        if (low[x] < tin[c])
            return 0;
        return x;
    };
    while (q--)
    {
        int a, b, c;
        std::cin >> a >> b >> c;
        if (a == c || b == c)
        {
            std::cout << "NO\n";
            continue;
        }
        if (group(c, a) == group(c, b))
            std::cout << "YES\n";
        else
            std::cout << "NO\n";
    }
    return 0;
}