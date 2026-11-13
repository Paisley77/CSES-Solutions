#include <iostream>
#include <vector>
#include <array>
#include <queue>

const int LOG = 20; // 2 ^ (LOG - 1) >= MAXN - 1

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, q;
    std::cin >> n >> q;
    std::vector<int> d(n + 1, n);
    std::queue<int> bfs;
    for (int i = 1; i <= n; ++i)
    {
        int c;
        std::cin >> c;
        if (c == 1)
        {
            d[i] = 0;
            bfs.push(i);
        }
    }
    std::vector<std::vector<int>> adj(n + 1);
    for (int i = 1; i <= n - 1; ++i)
    {
        int u, v;
        std::cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    while (!bfs.empty())
    {
        int u = bfs.front();
        bfs.pop();
        for (int v : adj[u])
        {
            if (d[v] > d[u] + 1)
            {
                d[v] = d[u] + 1;
                bfs.push(v);
            }
        }
    }
    std::vector<int> depth(n + 1, -1);
    std::vector<std::array<int, LOG>> up(n + 1);
    std::vector<std::array<int, LOG>> mn(n + 1);
    // Fill in depth and up[v][0]
    depth[1] = 0;
    up[1][0] = 1;
    bfs.push(1);
    while (!bfs.empty())
    {
        int u = bfs.front();
        bfs.pop();
        for (int v : adj[u])
        {
            if (depth[v] != -1)
                continue;
            depth[v] = depth[u] + 1;
            up[v][0] = u;
            bfs.push(v);
        }
    }
    // Fill in up[u][j] and mn[u][j]
    for (int j = 1; j < LOG; ++j)
    {
        for (int u = 1; u <= n; ++u)
        {
            int mid = up[u][j - 1];
            up[u][j] = up[mid][j - 1];
        }
    }
    for (int u = 1; u <= n; ++u)
    {
        mn[u][0] = std::min(d[u], d[up[u][0]]);
    }
    // mn[1][j] = d[1] for all j
    for (int j = 1; j < LOG; ++j)
    {
        for (int u = 1; u <= n; ++u)
        {
            int mid = up[u][j - 1];
            mn[u][j] = std::min(mn[u][j - 1], mn[mid][j - 1]);
        }
    }
    auto pathQuery = [&](int a, int b) -> std::pair<int, int>
    {
        /*
        Returns LCA(a, b), and min_{v \in P(a,b)} d[v]
        */
        int best = std::min(d[a], d[b]);
        if (depth[a] < depth[b])
            std::swap(a, b);
        // depth[a] >= depth[b]
        int diff = depth[a] - depth[b];
        for (int j = LOG - 1; j >= 0; --j)
        {
            if (diff & (1 << j))
            {
                best = std::min(mn[a][j], best);
                a = up[a][j];
            }
        }
        // depth[a] = depth[b] at this point
        if (a == b)
            return {a, best};
        // currently, a and b are two different points at the same depth
        // best stores the min d[v] among b and all v along the path leading a to the same depth as b
        for (int j = LOG - 1; j >= 0; --j)
        {
            if (up[a][j] != up[b][j])
            {
                best = std::min(best, mn[a][j]);
                best = std::min(best, mn[b][j]);
                a = up[a][j];
                b = up[b][j];
            }
        }
        // both a and b are now immediate children of their LCA
        best = std::min(mn[a][0], best);
        return {up[a][0], best};
    };
    while (q--)
    {
        int a, b;
        std::cin >> a >> b;
        auto [lca, best] = pathQuery(a, b);
        int dist = depth[a] + depth[b] - 2 * depth[lca];
        std::cout << dist + 2 * best << '\n';
    }
    return 0;
}