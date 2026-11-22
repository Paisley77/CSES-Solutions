#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<int>> graph(n + 1);
    for (int e = 0; e < m; ++e)
    {
        int u, v;
        std::cin >> u >> v;
        graph[u].push_back(v);
    }
    std::vector<int> dfn(n + 1, 0);
    std::vector<int> vertex(n + 1);
    std::vector<int> parent(n + 1, 0);
    // Assign DFS numbers
    std::vector<int> next(n + 1, 0);
    std::vector<int> stk;
    int timer = 1;
    dfn[1] = 1;
    vertex[1] = 1;
    stk.reserve(n);
    stk.push_back(1);
    while (!stk.empty())
    {
        int u = stk.back();
        if (next[u] == (int)graph[u].size())
        {
            stk.pop_back();
            continue;
        }
        int v = graph[u][next[u]++];
        if (dfn[v] != 0)
            continue;
        dfn[v] = ++timer;
        vertex[timer] = v;
        parent[timer] = dfn[u];
        stk.push_back(v);
    }
    int N = timer;
    // pred[v] stores the DFS numbers of all vertices u such that (u, v) is an edge in the original graph
    std::vector<std::vector<int>> pred(N + 1);
    for (int u = 1; u <= n; ++u)
    {
        if (dfn[u] == 0)
            continue;
        for (int v : graph[u])
            pred[dfn[v]].push_back(dfn[u]);
    }
    // Semidominators : initially set semi[v] = v
    std::vector<int> semi(N + 1);
    std::iota(semi.begin(), semi.end(), 0);
    // Immediate dominators: edges of the dominator tree
    std::vector<int> idom(N + 1, 0);
    // bucket[p] dynamically stores vertices with semi[v] = p
    std::vector<std::vector<int>> bucket(N + 1);
    std::vector<int> ancestor(N + 1, 0);
    // label[v] stores the vertex t with the minimum semidominator value from v (inclusive) to ancestor[v] (exclusive)
    // After linking v to its DFS parent, P(ancestor[v], v] = {v}, so label[v] = v remains correct.
    std::vector<int> label(N + 1);
    std::iota(label.begin(), label.end(), 0);
    // Auxiliary tree path that will store all DFS ancestors of vertex v that has already been processed in reverse DFS order
    // This variable serves as a temporary buffer
    std::vector<int> path;
    path.reserve(N);
    /*
    Returns the vertex t with the minimum semidominator value from the v (inclusive) to its auxiliary root (exclusive)
    After path compression, the ancestor of v will be set to its auxiliary root, whose ancestor is 0.
    */
    auto eval = [&](int v)
    {
        if (ancestor[v] == 0 || ancestor[ancestor[v]] == 0)
        {
            // ancestor[v] == 0 ==> P(root, v] = empty; v has not been linked into the auxiliary tree
            // ancestor[ancestor[v]] == 0 ==> P(root, v] = {v}; ancestor[v] has not been linked into the auxiliary tree
            // In either case, return the label of vertex v itself
            return label[v];
        }
        path.clear();
        int x = v;
        while (ancestor[x] != 0 && ancestor[ancestor[x]] != 0)
        {
            path.push_back(x);
            x = ancestor[x];
        }
        // 0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7
        // path = {7, 6, 5, 4, 3}
        // vertex 1 is not in the auxiliary tree since its ancestor is 0
        for (int i = (int)path.size() - 1; i >= 0; --i)
        {
            int y = path[i];
            int a = ancestor[y];
            if (semi[label[a]] < semi[label[y]])
                label[y] = label[a];
            ancestor[y] = ancestor[a];
        }
        // after the iteration, label[v] stores the vertex with smallest semi value from the auxiliary root (exclusive) to v (inclusive)
        return label[v];
    };
    // Compute semidominators & provisional immediate dominators
    for (int w = N; w >= 2; --w)
    {
        for (int v : pred[w])
        {
            // if v < w, then u = v \in A_w
            // if v > w, then semi[u] \in B_w
            int u = eval(v);
            semi[w] = std::min(semi[w], semi[u]);
        }
        // Link w into the auxiliary tree
        ancestor[w] = parent[w];
        // Add w to its bucket
        bucket[semi[w]].push_back(w);
        // Every vertex v will be processed from its bucket once
        for (int v : bucket[parent[w]])
        {
            // Since v has been added to the bucket, it must have been processed and its semi hence has been correctly computed
            // t is the vertex with min semi value along the DFS path from semi[v] (exclusive) to v (inclusive)
            // since t >= w, semi[t] has been correctly computed
            int t = eval(v);
            if (semi[t] == semi[v])
                idom[v] = semi[v];
            else
                idom[v] = t; // provisional; t must be strictly greater than semi[v] and strictly smaller than v in this case
        }
        bucket[parent[w]].clear();
    }
    // Resolving provisional immeidate dominators to the correct ones
    for (int w = 2; w <= N; ++w)
    {
        if (idom[w] != semi[w]) // must be the second case where semi[t] < semi[w]; then t < w
            idom[w] = idom[idom[w]];
    }
    std::vector<int> ans;
    int v = dfn[n];
    while (v != 0)
    {
        ans.push_back(vertex[v]);
        v = idom[v];
    }
    std::sort(ans.begin(), ans.end());
    std::cout << (int)ans.size() << '\n';
    for (int &v : ans)
        std::cout << v << '\n';
    return 0;
}