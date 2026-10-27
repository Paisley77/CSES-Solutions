#include <iostream>
#include <vector>
#include <numeric>

typedef long long int64;

const int MOD = 1e9 + 7;

class DSU
{
public:
    DSU(int n) : parent(n), sz(n, 1) { std::iota(parent.begin(), parent.end(), 0); }
    int find(int u)
    {
        if (parent[u] == u)
            return u;
        return parent[u] = find(parent[u]);
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

private:
    std::vector<int> parent;
    std::vector<int> sz;
};

int64 pow(int64 b, int64 e)
{
    int64 r = 1;
    while (e != 0)
    {
        if (e & 1)
            r = r * b % MOD;
        b = b * b % MOD;
        e >>= 1;
    }
    return r;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, m;
    std::cin >> n >> m;
    DSU dsu(n);
    for (int i = 0; i < m; ++i)
    {
        int u, v;
        std::cin >> u >> v;
        dsu.unite(--u, --v);
    }
    int64 c = 0;
    for (int v = 0; v < n; ++v)
        if (dsu.find(v) == v)
            c++;
    int64 nEulerian = pow(2, m - n + c);
    std::cout << nEulerian << '\n';
    return 0;
}