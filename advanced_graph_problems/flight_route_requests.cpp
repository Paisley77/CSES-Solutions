#include <iostream>
#include <vector>
#include <numeric>
#include <queue>

class DSU
{
public:
    explicit DSU(int n) : parent(n), sz(n, 1), components(n)
    {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int u)
    {
        if (u != parent[u])
            return parent[u] = find(parent[u]);
        return u;
    }

    void unite(int u, int v)
    {
        u = find(u);
        v = find(v);
        if (u == v)
            return;
        if (sz[u] < sz[v])
            std::swap(u, v);
        parent[v] = u;
        sz[u] += sz[v];
        components--;
    }

    int count() { return components; }

private:
    std::vector<int> parent;
    std::vector<int> sz;
    int components;
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, m;
    std::cin >> n >> m;
    DSU dsu(n);
    std::vector<std::vector<int>> children(n);
    std::vector<int> indegree(n, 0);
    for (int i = 0; i < m; ++i)
    {
        int u, v;
        std::cin >> u >> v;
        --u;
        --v;
        dsu.unite(u, v);
        children[u].push_back(v);
        indegree[v]++;
    }
    std::queue<int> q;
    for (int u = 0; u < n; ++u)
        if (indegree[u] == 0)
            q.push(u);
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (int v : children[u])
        {
            indegree[v]--;
            if (indegree[v] == 0)
                q.push(v);
        }
    }
    std::vector<char> cyclic(n, 0);
    for (int u = 0; u < n; ++u)
    {
        if (indegree[u] > 0)
            cyclic[dsu.find(u)] = 1;
    }
    std::cout << n - dsu.count() + std::accumulate(cyclic.begin(), cyclic.end(), 0);
    return 0;
}