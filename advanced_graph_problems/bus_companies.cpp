#include <vector>
#include <queue>
#include <iostream>

using ll = long long;
using State = std::pair<ll, int>;

const ll INF = 1e15;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, m;
    std::cin >> n >> m;
    std::vector<ll> c(m);
    for (auto &x : c)
        std::cin >> x;
    std::vector<std::vector<int>> city2Co(n), co2City(m);
    for (int i = 0; i < m; ++i)
    {
        int k;
        std::cin >> k;
        for (int j = 0; j < k; ++j)
        {
            int a;
            std::cin >> a;
            co2City[i].push_back(--a);
            city2Co[a].push_back(i);
        }
    }
    std::vector<char> processed(m, 0);
    std::vector<ll> dist(n, INF);
    dist[0] = 0;
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq; // min at the top
    pq.push({0, 0});
    while (!pq.empty())
    {
        auto [d, city] = pq.top();
        pq.pop();
        if (d != dist[city])
            continue;
        for (int co : city2Co[city])
        {
            if (processed[co])
                continue;
            processed[co] = 1;
            ll newDist = d + c[co];
            for (int nc : co2City[co])
            {
                if (dist[nc] > newDist)
                {
                    dist[nc] = newDist;
                    pq.push({newDist, nc});
                }
            }
        }
    }
    for (int i = 0; i < n; ++i)
        std::cout << dist[i] << '\n';
    return 0;
}