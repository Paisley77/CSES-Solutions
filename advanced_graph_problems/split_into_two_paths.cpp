#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>

long long hash(int u, int v) { return ((long long)u << 32) | v; }
int assert_fail()
{
    std::cout << "NO\n";
    return 0;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    std::vector<std::vector<int>> adj(n + 1), radj(n + 1);
    std::unordered_set<long long> edge_set;
    edge_set.reserve(m);
    std::vector<int> in_degree(n + 1, 0);
    for (int i = 0; i < m; ++i)
    {
        int u, v;
        std::cin >> u >> v;
        adj[u].push_back(v);
        radj[v].push_back(u);
        edge_set.insert(hash(u, v));
        in_degree[v]++;
    }

    std::queue<int> q;
    for (int i = 1; i <= n; ++i)
        if (in_degree[i] == 0)
            q.push(i);
    std::vector<int> v_order;
    v_order.push_back(0);
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        v_order.push_back(u);
        for (int v : adj[u])
        {
            if (--in_degree[v] == 0)
                q.push(v);
        }
    }
    std::vector<int> id(n + 1, 0);
    for (int i = 1; i <= n; ++i)
        id[v_order[i]] = i;

    std::vector<char> in_set(n + 1, 0);
    std::vector<int> active_nodes;
    std::vector<int> parent(n + 1, -1);
    in_set[0] = 1;
    active_nodes.push_back(0);
    for (int i = 1; i < n; ++i)
    {
        int u = v_order[i], v = v_order[i + 1], match_idx = -1;
        if (in_set[0])
            match_idx = 0;
        else
        {
            for (int w : radj[v])
                if (in_set[id[w]])
                {
                    match_idx = id[w];
                    break;
                }
        }
        if (match_idx != -1)
            parent[i + 1] = match_idx;
        bool has_edge = edge_set.count(hash(u, v)), has_match = (match_idx != -1);
        if (!has_edge)
        {
            for (int x : active_nodes)
                in_set[x] = 0;
            active_nodes.clear();
            if (!has_match)
                return assert_fail();
        }
        if (has_match)
        {
            in_set[i] = 1;
            active_nodes.push_back(i);
        }
    }

    int final_j = active_nodes.front();
    int cur_j = final_j, cur_i = n;
    std::vector<int> pred(n + 1, 0);
    while (cur_i > 1)
    {
        if (cur_j == cur_i - 1)
        {
            int k = parent[cur_i];
            pred[v_order[cur_i]] = v_order[k];
            cur_j = k;
            cur_i--;
        }
        else
        {
            pred[v_order[cur_i]] = v_order[cur_i - 1];
            cur_i--;
        }
    }
    pred[v_order[1]] = 0;

    std::vector<int> path_1;
    for (int u = v_order[n]; u != 0; u = pred[u])
        path_1.push_back(u);
    std::reverse(path_1.begin(), path_1.end());

    std::vector<int> path_2;
    if (final_j > 0)
    {
        for (int v = v_order[final_j]; v != 0; v = pred[v])
            path_2.push_back(v);
        std::reverse(path_2.begin(), path_2.end());
    }

    std::cout << "YES\n";
    std::cout << path_1.size() << " ";
    for (int u : path_1)
        std::cout << u << " ";
    std::cout << '\n';
    std::cout << path_2.size() << " ";
    for (int v : path_2)
        std::cout << v << " ";
    std::cout << '\n';

    return 0;
}