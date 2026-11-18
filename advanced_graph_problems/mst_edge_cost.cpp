#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

typedef long long int64;

struct Edge
{
    int u;
    int v;
    int64 w;
    int id;
    bool operator<(const Edge &other) const { return w < other.w; }
};

class DSU
{
public:
    explicit DSU(int n) : parent(n + 1), sz(n + 1, 1)
    {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int u)
    {
        if (u != parent[u])
            return parent[u] = find(parent[u]);
        return u;
    }

    bool unite(int u, int v)
    {
        u = find(u);
        v = find(v);
        if (u == v)
            return false;
        if (sz[u] < sz[v])
            std::swap(u, v);
        parent[v] = u;
        sz[u] += sz[v];
        return true;
    }

private:
    std::vector<int> parent;
    std::vector<int> sz;
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
    std::sort(edges.begin(), edges.end());
    DSU dsu(n);
    std::vector<std::vector<std::pair<int, int>>> tree(n + 1);
    int64 mstCost = 0;
    for (const Edge &e : edges)
    {
        if (dsu.unite(e.u, e.v))
        {
            mstCost += e.w;
            tree[e.u].push_back({e.v, e.w});
            tree[e.v].push_back({e.u, e.w});
        }
    }
    int LOG = 1;
    while (1 << (LOG - 1) < (n - 1))
        ++LOG;
    std::vector<std::vector<int>> up(n + 1, std::vector<int>(LOG));
    std::vector<std::vector<int64>> mx(n + 1, std::vector<int64>(LOG, 0));
    std::vector<int> depth(n + 1, -1);
    std::vector<int> order;
    order.reserve(n);
    order.push_back(1);
    depth[1] = 0;
    up[1][0] = 1;
    for (int i = 0; i < (int)order.size(); ++i)
    {
        int u = order[i];
        for (auto [v, w] : tree[u])
        {
            if (depth[v] != -1)
                continue;
            depth[v] = depth[u] + 1;
            up[v][0] = u;
            mx[v][0] = w;
            order.push_back(v);
        }
    }
    for (int j = 1; j < LOG; ++j)
    {
        for (int u : order)
        {
            int mid = up[u][j - 1];
            up[u][j] = up[mid][j - 1];
            mx[u][j] = std::max(mx[u][j - 1], mx[mid][j - 1]);
        }
    }
    auto query_path = [&](int u, int v) -> int64
    {
        int64 best = 0;
        if (depth[u] < depth[v])
            std::swap(u, v);
        int diff = depth[u] - depth[v];
        for (int j = LOG - 1; j >= 0; --j)
        {
            if ((1 << j) & diff)
            {
                best = std::max(mx[u][j], best);
                u = up[u][j];
            }
        }
        if (u == v)
            return best;
        for (int j = LOG - 1; j >= 0; --j)
        {
            if (up[u][j] != up[v][j])
            {
                best = std::max(mx[u][j], best);
                best = std::max(mx[v][j], best);
                u = up[u][j];
                v = up[v][j];
            }
        }
        best = std::max(mx[u][0], best);
        best = std::max(mx[v][0], best);
        return best;
    };
    std::vector<int64> ans(m);
    for (int i = 0; i < m; ++i)
    {
        Edge e = edges[i];
        ans[e.id] = mstCost + e.w - query_path(e.u, e.v);
    }
    for (int64 &c : ans)
        std::cout << c << '\n';
    return 0;
}