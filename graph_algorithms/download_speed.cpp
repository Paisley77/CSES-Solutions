#include <iostream>
#include <vector>
#include <queue>

const long long INF = 1e18;

int n, m;
struct Edge
{
    int to;
    long long cap;
};
std::vector<Edge> edges;
std::vector<std::vector<int>> g;
std::vector<int> level, iter;

void add_edge(int a, int b, long long c)
{
    g[a].push_back(edges.size());
    edges.push_back({b, c});
    g[b].push_back(edges.size());
    edges.push_back({a, 0});
}

bool bfs(int s, int t)
{
    level.assign(n + 1, -1);
    level[s] = 0;
    std::queue<int> q;
    q.push(s);
    while (!q.empty())
    {
        int v = q.front();
        q.pop();
        for (int id : g[v])
        {
            if (edges[id].cap > 0 && level[edges[id].to] < 0)
            {
                level[edges[id].to] = level[v] + 1;
                q.push(edges[id].to);
            }
        }
    }
    return level[t] >= 0;
}

long long dfs(int s, int t, long long f)
{
    if (s == t)
        return f;
    for (int &i = iter[s]; i < (int)g[s].size(); ++i)
    {
        int id = g[s][i];
        Edge &e = edges[id];
        if (e.cap > 0 && level[e.to] == level[s] + 1)
        {
            long long d = dfs(e.to, t, std::min(e.cap, f));
            if (d > 0)
            {
                e.cap -= d;
                edges[id ^ 1].cap += d;
                return d;
            }
        }
    }
    return 0;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::cin >> n >> m;
    if (n == 1)
    {
        std::cout << 0 << '\n';
        return 0;
    }

    g.assign(n + 1, {});
    int a, b;
    long long c;
    for (int i = 0; i < m; ++i)
    {
        std::cin >> a >> b >> c;
        add_edge(a, b, c);
    }

    long long max_flow = 0;
    while (bfs(1, n))
    {
        iter.assign(n + 1, 0);
        long long f;
        while ((f = dfs(1, n, INF)) > 0)
            max_flow += f;
    }
    std::cout << max_flow << '\n';
    return 0;
}