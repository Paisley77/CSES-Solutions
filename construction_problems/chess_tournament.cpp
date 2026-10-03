#include <iostream>
#include <vector>
#include <queue>

int print_no()
{
    std::cout << "IMPOSSIBLE" << std::endl;
    return 0;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    std::vector<int> x(n + 1);
    int sum = 0;
    for (int i = 1; i <= n; ++i)
    {
        std::cin >> x[i];
        if (x[i] < 0 || x[i] >= n)
            return print_no();
        sum += x[i];
    }
    if (sum % 2 != 0)
        return print_no();
    std::priority_queue<std::pair<int, int>> pq;
    for (int i = 1; i <= n; ++i)
        if (x[i] > 0)
            pq.push({x[i], i});
    std::vector<std::pair<int, int>> edges, temp;
    edges.reserve(sum / 2);

    while (!pq.empty())
    {
        auto [dv, v] = pq.top();
        pq.pop();
        temp.clear();
        for (int i = 0; i < dv; ++i)
        {
            if (pq.empty())
                return print_no();
            auto [du, u] = pq.top();
            pq.pop();
            edges.push_back({v, u});
            temp.push_back({du - 1, u});
        }
        for (auto &p : temp)
            if (p.first > 0)
                pq.push(p);
    }

    std::cout << edges.size() << '\n';
    for (auto &[v, u] : edges)
        std::cout << v << " " << u << '\n';

    return 0;
}