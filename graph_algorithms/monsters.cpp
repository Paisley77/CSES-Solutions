#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    std::vector<std::string> g(n);
    for (int i = 0; i < n; ++i)
        std::cin >> g[i];

    const int INF = 1 << 30;
    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};
    const char dc[4] = {'D', 'U', 'L', 'R'};

    auto id = [m](int i, int j)
    { return i * m + j; };

    std::vector<int> mon(n * m, INF), dist(n * m, INF);
    std::vector<signed char> pdir(n * m, -1);

    std::queue<int> q;
    int start = -1;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
        {
            if (g[i][j] == 'M')
            {
                mon[id(i, j)] = 0;
                q.push(id(i, j));
            }
            else if (g[i][j] == 'A')
                start = id(i, j);
        }

    // First BFS: Monster (multiple sources)
    while (!q.empty())
    {
        int c = q.front();
        q.pop();
        int i = c / m, j = c % m;
        for (int d = 0; d < 4; ++d)
        {
            int ni = i + dx[d], nj = j + dy[d];
            if (ni < 0 || ni >= n || nj < 0 || nj >= m)
                continue;
            if (g[ni][nj] == '#' || mon[id(ni, nj)] != INF)
                continue;
            mon[id(ni, nj)] = mon[c] + 1;
            q.push(id(ni, nj));
        }
    }

    // Second BFS: Self
    int escape = -1;
    if (mon[start] > 0)
    {
        dist[start] = 0;
        q.push(start);
    }
    while (!q.empty())
    {
        int c = q.front();
        q.pop();
        int i = c / m, j = c % m;
        if (i == 0 || i == (n - 1) || j == 0 || j == (m - 1))
        {
            escape = c;
            break;
        }
        for (int d = 0; d < 4; ++d)
        {
            int ni = i + dx[d], nj = j + dy[d];
            if (ni < 0 || ni >= n || nj < 0 || nj >= m)
                continue;
            if (g[ni][nj] == '#')
                continue;
            int nc = id(ni, nj);
            if (dist[nc] != INF)
                continue;
            int vt = dist[c] + 1;
            if (mon[nc] <= vt)
                continue;
            dist[nc] = vt;
            pdir[nc] = static_cast<signed char>(d);
            q.push(nc);
        }
    }

    if (escape == -1)
    {
        std::cout << "NO\n";
        return 0;
    }

    std::string path;
    for (int c = escape; c != start;)
    {
        int d = pdir[c];
        path += dc[d];
        c = id(c / m - dx[d], c % m - dy[d]);
    }
    std::reverse(path.begin(), path.end());

    std::cout << "YES\n"
              << path.size() << '\n'
              << path << '\n';
    return 0;
}