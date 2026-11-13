#include <iostream>
#include <vector>
#include <array>
#include <queue>

const int LOG = 20;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, q;
    std::cin >> n >> q;
    std::vector<std::vector<int>> adj(n + 1);
    std::vector<int> coined(n + 1, 0);
    std::vector<int> degree(n + 1, 0);
    for (int i = 1; i <= n; ++i)
        std::cin >> coined[i];
    for (int i = 1; i < n; ++i)
    {
        int a, b;
        std::cin >> a >> b;
        adj[a].push_back(b);
        adj[b].push_back(a);
        degree[a]++;
        degree[b]++;
    }
    std::vector<char> removed(n + 1, 0);
    // Find minimal connected subtree containing all coins
    // Invariant: T is a connected subtree containing all coins
    // After the last iteration, every leaf node contanis a coin
    // Sufficient & Necessary for minimality
    std::queue<int> que;
    for (int u = 1; u <= n; ++u)
        if (!coined[u] && degree[u] == 1)
            que.push(u);
    int coreSize = n;
    while (!que.empty())
    {
        int u = que.front();
        que.pop();
        removed[u] = 1;
        coreSize--;
        for (int v : adj[u])
        {
            if (removed[v])
                continue;
            if (--degree[v] == 1 && !coined[v])
                que.push(v);
        }
    }
    int E = coreSize - 1;
    // For each node v, find the unique node p[v] in T closest to v;
    // Calculate d[v] := dist(v, p[v])
    std::vector<int> p(n + 1, 0);
    std::vector<int> d(n + 1, -1);
    for (int i = 1; i <= n; ++i)
    {
        if (!removed[i])
        {
            p[i] = i;
            d[i] = 0;
            que.push(i);
        }
    }
    while (!que.empty())
    {
        int u = que.front();
        que.pop();
        for (int v : adj[u])
        {
            if (p[v] > 0)
                continue;
            p[v] = p[u];
            d[v] = d[u] + 1;
            que.push(v);
        }
    }
    // Calculate LCA using binary lifting
    std::vector<int> depth(n + 1, -1);
    std::vector<std::array<int, LOG>> up(n + 1);
    depth[1] = 0;
    up[1][0] = 1;
    que.push(1);
    while (!que.empty())
    {
        int u = que.front();
        que.pop();
        for (int v : adj[u])
        {
            if (depth[v] != -1)
                continue;
            depth[v] = depth[u] + 1;
            up[v][0] = u;
            que.push(v);
        }
    }
    for (int j = 1; j < LOG; ++j)
    {
        for (int v = 1; v <= n; ++v)
        {
            int mid = up[v][j - 1];
            up[v][j] = up[mid][j - 1];
        }
    }
    auto find_lca = [&](int a, int b) -> int
    {
        if (depth[a] < depth[b])
            std::swap(a, b);
        int diff = depth[a] - depth[b];
        for (int j = LOG - 1; j >= 0; --j)
        {
            if (diff & (1 << j))
                a = up[a][j];
        }
        if (a == b)
            return a;
        for (int j = LOG - 1; j >= 0; --j)
        {
            if (up[a][j] != up[b][j])
            {
                a = up[a][j];
                b = up[b][j];
            }
        }
        return up[a][0];
    };
    auto dist = [&](int u, int v) -> int { // O(log n)
        int lca = find_lca(u, v);
        return depth[u] + depth[v] - 2 * depth[lca];
    };
    // Process query
    while (q--)
    {
        int a, b;
        std::cin >> a >> b;
        int u = p[a], v = p[b];
        std::cout << d[a] + d[b] + 2 * E - dist(u, v) << '\n';
    }
    return 0;
}