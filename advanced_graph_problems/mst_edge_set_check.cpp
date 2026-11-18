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
    bool operator<(const Edge &other) const
    {
        if (w == other.w)
            return id < other.id;
        return w < other.w;
    }
};

class RollbackDSU
{
public:
    explicit RollbackDSU(int n) : parent(n + 1), sz(n + 1, 1)
    {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int u)
    {
        if (parent[u] != u)
            return find(parent[u]);
        return u;
    }

    void unite(int u, int v)
    {
        u = find(u);
        v = find(v);
        if (u == v)
        {
            history.push_back({0, 0});
            return;
        }
        if (sz[u] < sz[v])
            std::swap(u, v);
        parent[v] = u;
        sz[u] += sz[v];
        history.push_back({u, v});
    }

    void rollback(int snapshot)
    {
        int delta = (int)history.size() - snapshot;
        for (int i = 0; i < delta; ++i)
        {
            auto [u, v] = history.back();
            history.pop_back();
            if (u == 0)
                continue;
            parent[v] = v;
            sz[u] -= sz[v];
        }
    }

    int snapshot() const { return (int)history.size(); }

private:
    std::vector<int> parent;
    std::vector<int> sz;
    std::vector<std::pair<int, int>> history;
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, m, q;
    std::cin >> n >> m >> q;
    std::vector<Edge> graph_edges(m);
    for (int i = 0; i < m; ++i)
    {
        int a, b, w;
        std::cin >> a >> b >> w;
        graph_edges[i] = {a, b, w, i};
    }
    std::vector<Edge> query_edges;
    query_edges.reserve(m);
    for (int i = 0; i < q; ++i)
    {
        int sz;
        std::cin >> sz;
        for (int j = 0; j < sz; ++j)
        {
            int edgeId;
            std::cin >> edgeId;
            Edge &e = graph_edges[edgeId - 1];
            query_edges.push_back({e.u, e.v, e.w, i});
        }
    }
    int totalQuery = (int)query_edges.size();
    std::sort(graph_edges.begin(), graph_edges.end());
    std::sort(query_edges.begin(), query_edges.end());
    RollbackDSU dsu(n);
    std::vector<char> ans(q, 1);
    int qi = 0;
    int oi = 0;
    while (qi < totalQuery)
    {
        // Permanently add all edges of weight < w
        int w = query_edges[qi].w;
        int oj = oi;
        while (oj < m && graph_edges[oj].w < w)
        {
            int u = graph_edges[oj].u, v = graph_edges[oj].v;
            dsu.unite(u, v);
            oj++;
        }
        oi = oj; // first index of the graph edge >= w

        // Retrieve all query edges of weight w
        int qj = qi + 1;
        while (qj < totalQuery && query_edges[qj].w == w)
            ++qj;
        int id = query_edges[qi].id;
        int snapshot = dsu.snapshot(); // number of edges smaller than weight w
        for (int k = qi; k < qj; ++k)  // all queries of the same weight
        {
            if (query_edges[k].id == id)
            { // same query set F_w
                if (!ans[id])
                    continue;
                int u = query_edges[k].u, v = query_edges[k].v;
                if (dsu.find(u) == dsu.find(v))
                {
                    ans[id] = 0;
                    continue;
                }
                dsu.unite(u, v);
                continue;
            }
            // Finished processing the F_w;
            // No matter how many edges were added in processing the last query set,
            // needs to rewind to keep only edges with weight less than w
            dsu.rollback(snapshot);
            // Reset for new query set processing
            id = query_edges[k].id;
            --k;
        }
        // make sure after processing the last query set of weight w, the dsu contains only edges of weight less than w
        dsu.rollback(snapshot);
        // Update query index to correspond to the next smallest weight
        qi = qj;
    }

    for (char &x : ans)
        std::cout << (x ? "YES" : "NO") << '\n';
    return 0;
}