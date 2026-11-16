#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

struct Edge
{
    int u;
    int v;
    int w;
    int id;
    bool operator<(const Edge &other) const { return w < other.w; }
};

class DSU
{
private:
    std::vector<int> parent;
    std::vector<int> sz;

public:
    explicit DSU(int n) : parent(n + 1), sz(n + 1, 1)
    {
        std::iota(parent.begin(), parent.end(), 0);
    }
    int find(int u)
    {
        if (u != parent[u])
            parent[u] = find(parent[u]);
        return parent[u];
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
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, m;
    std::cin >> n >> m;
    std::vector<Edge> edges(m);
    for (int i = 0; i < m; ++i)
    {
        int u, v, w;
        std::cin >> u >> v >> w;
        edges[i] = {u, v, w, i};
    }
    // nodes are 1-indexed; edges are 0-indexed
    std::sort(edges.begin(), edges.end());
    DSU dsu(n);
    std::vector<char> ans(m, 0);
    int i = 0;
    while (i < m)
    {
        int j = i;
        while (j < m && edges[j].w == edges[i].w)
            ++j;
        for (int k = i; k < j; ++k)
        {
            int u = edges[k].u;
            int v = edges[k].v;
            if (dsu.find(u) != dsu.find(v))
                ans[edges[k].id] = 1;
        }
        for (int k = i; k < j; ++k)
            dsu.unite(edges[k].u, edges[k].v);
        i = j;
    }
    for (char &a : ans)
        std::cout << (a ? "YES" : "NO") << '\n';
    return 0;
}