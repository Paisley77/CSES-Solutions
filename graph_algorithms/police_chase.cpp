#include <iostream>
#include <vector>
#include <queue>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<int>> adj(n);
    std::vector<std::vector<int>> cap(n, std::vector<int>(n, 0));
    for (int i = 0; i < m; ++i)
    {
        int a, b;
        std::cin >> a >> b;
        adj[a - 1].push_back(b - 1);
        adj[b - 1].push_back(a - 1);
        cap[a - 1][b - 1]++;
        cap[b - 1][a - 1]++;
    }
    std::vector<int> parent(n);
    int flow = 0;
    auto bfs = [&](int s = 0)
    {
        std::queue<int> q;
        q.push(s);
        std::fill(parent.begin(), parent.end(), -1);
        parent[s] = s;
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            for (int v : adj[u])
            {
                if (parent[v] == -1 && cap[u][v] > 0)
                {
                    parent[v] = u;
                    q.push(v);
                }
            }
        }
        return parent[n - 1] != -1;
    };

    auto flowUpdate = [&]()
    {
        int v = n - 1;
        while (v != 0)
        {
            cap[parent[v]][v]--; // reduce residual capacity by 1
            cap[v][parent[v]]++;
            v = parent[v];
        }
        flow++;
    };

    while (bfs())
        flowUpdate();

    std::vector<char> reachable(n, 0);
    reachable[0] = 1;
    auto partition = [&]()
    {
        std::queue<int> q;
        q.push(0);
        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            for (int v : adj[u])
            {
                if (reachable[v] == 0 && cap[u][v] > 0)
                {
                    reachable[v] = 1;
                    q.push(v);
                }
            }
        }
    };
    partition();
    std::vector<std::pair<int, int>> cross_edges;
    for (int i = 0; i < n; ++i)
        if (reachable[i] == 1)
            for (int j : adj[i])
                if (reachable[j] == 0)
                    cross_edges.push_back({i + 1, j + 1});

    std::cout << cross_edges.size() << '\n';
    for (auto &p : cross_edges)
    {
        std::cout << p.first << " " << p.second << '\n';
    }
    return 0;
}