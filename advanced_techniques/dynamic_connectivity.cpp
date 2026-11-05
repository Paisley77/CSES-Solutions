#include <iostream>
#include <vector>
#include <unordered_map>

typedef long long int64;

const int NODE_BITS = 20;

struct Edge
{
    int u;
    int v;
};

class DSU
{
private:
    std::vector<int> parent;
    std::vector<int> sz;
    int components;
    struct Heart
    {
        int childRoot;
        int parentRoot;
    };
    std::vector<Heart> history;

public:
    explicit DSU(int n) : parent(n + 1), sz(n + 1, 1), components(n)
    {
        for (int i = 1; i <= n; ++i)
            parent[i] = i;
    }

    int find(int u)
    {
        while (parent[u] != u)
            u = parent[u];
        return u;
    }

    void unite(int u, int v)
    {
        u = find(u);
        v = find(v);
        if (u == v)
        {
            history.push_back({-1, -1});
            return;
        }
        if (sz[u] < sz[v])
            std::swap(u, v);
        parent[v] = u;
        sz[u] += sz[v];
        --components;
        history.push_back({v, u});
    }

    void rollback(int snap)
    {
        while ((int)history.size() > snap)
        {
            auto [v, u] = history.back();
            history.pop_back();
            if (v == -1)
                continue;
            parent[v] = v;
            sz[u] -= sz[v];
            ++components;
        }
    }

    int snapshot() { return (int)history.size(); }

    int num_components() { return components; }
};

class SegmentTree
{
private:
    struct Node
    {
        std::vector<Edge> edges;
    };
    std::vector<Node> tree;

public:
    std::vector<int> comp_t;

    SegmentTree(int T) : tree(4 * T + 1), comp_t(T) {}

    void insert(int node, int left, int right, int qleft, int qright, Edge edge)
    {
        if (qleft > right || qright < left)
            return;
        if (qleft <= left && qright >= right)
        {
            // touches the interval [left, right] contained in the time interval
            tree[node].edges.push_back(edge);
            return;
        }
        int mid = (left + right) >> 1;
        insert(2 * node, left, mid, qleft, qright, edge);
        insert(2 * node + 1, mid + 1, right, qleft, qright, edge);
    }

    void solve(int node, int left, int right, DSU &dsu)
    {
        int snap = dsu.snapshot();
        for (const Edge &edge : tree[node].edges)
            dsu.unite(edge.u, edge.v);
        if (left == right)
        {
            // Reached a leaf node corresponding to time t;
            // so every edge currently in the dsu exists at time t.
            comp_t[left] = dsu.num_components();
        }
        else
        {
            int mid = (left + right) >> 1;
            solve(2 * node, left, mid, dsu);
            solve(2 * node + 1, mid + 1, right, dsu);
        }
        dsu.rollback(snap);
    }
};

int64 edgeKey(int u, int v)
{
    // returns a 40-bit edge key
    if (u > v)
        std::swap(u, v); // normalize endpoints
    return ((int64)u << NODE_BITS) + (int64)v;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, m, k;
    std::cin >> n >> m >> k;
    std::unordered_map<int64, int> active;
    std::unordered_map<int64, Edge> key2edge;
    active.reserve(m + k);
    key2edge.reserve(m + k);
    SegmentTree segTree(k + 1);
    for (int i = 0; i < m; ++i)
    {
        int u, v;
        std::cin >> u >> v;
        int64 key = edgeKey(u, v);
        active[key] = 0;
        key2edge[key] = {u, v};
    }
    for (int t = 1; t <= k; ++t)
    {
        int type, u, v;
        std::cin >> type >> u >> v;
        if (type == 1)
        {
            int64 key = edgeKey(u, v);
            active[key] = t;
            key2edge[key] = {u, v};
        }
        else
        {
            int64 key = edgeKey(u, v);
            int start = active[key];
            segTree.insert(1, 0, k, start, t - 1, {u, v});
            active.erase(key);
        }
    }
    for (auto [key, start] : active)
    {
        segTree.insert(1, 0, k, start, k, key2edge[key]);
    }
    DSU dsu(n);
    segTree.solve(1, 0, k, dsu);
    for (int c : segTree.comp_t)
        std::cout << c << '\n';
    return 0;
}