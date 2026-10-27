#include <iostream>
#include <vector>

std::vector<std::vector<int>> graph;
std::vector<unsigned char> seen;
std::vector<int> matchV;

bool dfs(int u)
{
    for (int v : graph[u])
    {
        if (seen[v])
            continue;
        seen[v] = 1;
        if (matchV[v] == -1 || dfs(matchV[v]))
        {
            matchV[v] = u;
            return true;
        }
    }
    return false;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, m, k;
    std::cin >> n >> m >> k;
    graph.resize(n);
    for (int i = 0; i < k; ++i)
    {
        int u, v;
        std::cin >> u >> v;
        graph[--u].push_back(--v);
    }
    matchV.assign(m, -1);
    int totalMatch = 0;
    for (int u = 0; u < n; ++u)
    {
        seen.assign(m, 0);
        if (dfs(u))
            ++totalMatch;
    }
    std::cout << totalMatch << '\n';
    for (int v = 0; v < m; ++v)
    {
        if (matchV[v] == -1)
            continue;
        std::cout << matchV[v] + 1 << " " << v + 1 << '\n';
    }
    return 0;
}
