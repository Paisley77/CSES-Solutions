#include <iostream>
#include <vector>
#include <queue>

const int MAXN = 2e5;
const int MAXE = 5e5;

int head[MAXN + 1];
int to_edge[MAXE];
int nxt_edge[MAXE];
int edge_cnt = 0;

int matchL[MAXN + 1];
int matchR[MAXN + 1];
int dist_[MAXN + 1];
int curHead[MAXN + 1];
std::vector<char> used;

void add_edge(int u, int v)
{
    to_edge[edge_cnt] = v;
    nxt_edge[edge_cnt] = head[u];
    head[u] = edge_cnt++;
}

bool bfs(int n)
{
    std::queue<int> q;
    for (int i = 1; i <= n; ++i)
    {
        if (matchL[i] == 0)
        {
            dist_[i] = 0;
            q.push(i);
        }
        else
            dist_[i] = MAXN;
    }
    dist_[0] = MAXN;
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        if (dist_[u] >= dist_[0])
            continue;
        for (int e = head[u]; e != -1; e = nxt_edge[e])
        {
            int v = to_edge[e];
            int w = matchR[v];
            if (dist_[w] == MAXN)
            {
                dist_[w] = dist_[u] + 1;
                q.push(w);
            }
        }
    }
    return dist_[0] < MAXN;
}

bool dfs(int u)
{
    for (int &e = curHead[u]; e != -1; e = nxt_edge[e])
    {
        int v = to_edge[e];
        if (used[v])
            continue;
        int w = matchR[v];
        if (w == 0 || dist_[w] == dist_[u] + 1)
            used[v] = 1;
        if (w == 0 || (dist_[w] == dist_[u] + 1 && dfs(w)))
        {
            matchL[u] = v;
            matchR[v] = u;
            return true;
        }
    }
    dist_[u] = MAXN;
    return false;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    for (int u = 1; u <= n; ++u)
        head[u] = -1;
    for (int i = 0; i < m; ++i)
    {
        int u, v;
        std::cin >> u >> v;
        add_edge(u, v);
    }

    int match = 0;
    while (bfs(n))
    {
        used.assign(n + 1, 0);
        for (int u = 1; u <= n; ++u)
            curHead[u] = head[u];
        for (int u = 1; u <= n; ++u)
        {
            if (matchL[u] == 0 && dfs(u))
                match++;
        }
    }

    int C = n - match;
    if (C > 2)
    {
        std::cout << "NO\n";
        return 0;
    }

    std::vector<std::vector<int>> paths(C);
    int idx = 0;
    for (int u = 1; u <= n; ++u)
    {
        if (matchR[u] != 0)
            continue;
        for (int cur = u; cur != 0; cur = matchL[cur])
            paths[idx].push_back(cur);
        if (C < 2)
            break;
        idx++;
    }

    std::cout << "YES\n";
    for (int idx = 0; idx < C; ++idx)
    {
        std::cout << paths[idx].size() << " ";
        for (int u : paths[idx])
            std::cout << u << " ";
        std::cout << '\n';
    }

    if (C < 2)
        std::cout << 0;

    return 0;
}