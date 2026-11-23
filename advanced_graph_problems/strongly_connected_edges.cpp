#include <vector>
#include <iostream>

int fail()
{
    std::cout << "IMPOSSIBLE\n";
    return 0;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<std::pair<int, int>>> graph(n + 1);
    for (int e = 0; e < m; ++e)
    {
        int u, v;
        std::cin >> u >> v;
        graph[u].push_back({v, e});
        graph[v].push_back({u, e});
    }
    /*
    For any vertex v not equal to 1, let u be its DFS parent. Then:
    (u, v) is a bridge <=> tin[v] = low[v] <=> low[v] > tin[u]
    Once every neighbor of v has been processed, low[v] must have been already finalized.
    Therefore, for every non-root vertex v, we can check for bridges after all its neighbors have been processed.
    Any non-tree edge cannot be a bridge. Thus we only need to consider tree edges.
    */
    std::vector<int> low(n + 1);
    std::vector<int> tin(n + 1, 0);
    std::vector<int> parent(n + 1, 0);
    std::vector<int> parentEdge(n + 1, 0);
    std::vector<int> next(n + 1, 0);
    std::vector<std::pair<int, int>> orn(m);
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
            if (parent[u] != 0)
            {
                int p = parent[u];
                // low[u] must have been finalized by now;
                // tin[p] must have been set too since we discover u after p
                if (low[u] == tin[u]) // alternatively, we can use the equivalent condition (low[u] > tin[p])
                    return fail();
                low[p] = std::min(low[p], low[u]);
            }
            continue;
        }
        auto [v, id] = graph[u][next[u]++];
        // v is a DFS parent of u
        if (id == parentEdge[u])
            continue;
        if (tin[v] == 0)
        {
            orn[id] = {u, v};
            parent[v] = u;
            parentEdge[v] = id;
            tin[v] = low[v] = ++timer;
            stk.push_back(v);
        }
        else if (tin[v] < tin[u])
        {
            // (u, v) is a non-tree edge and v is a DFS ancestor of u
            orn[id] = {u, v};
            low[u] = std::min(tin[v], low[u]);
        }
        // if tin[v] > tin[u], then the above conditional-branch will be triggered
        // when exploring v and orn[id] and low[v] will be set accordingly
    }
    if (timer < n)
        return fail();
    for (auto [u, v] : orn)
        std::cout << u << " " << v << '\n';
    return 0;
}