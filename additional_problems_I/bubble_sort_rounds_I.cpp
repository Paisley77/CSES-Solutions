#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    std::vector<std::pair<int, int>> a(n);
    int i = 0;
    for (auto &p : a)
    {
        std::cin >> p.first;
        p.second = i++;
    }
    std::sort(a.begin(), a.end());
    int ans = 0;
    for (int i = 0; i < n; ++i)
    {
        int steps = a[i].second - i;
        if (steps > ans)
            ans = steps;
    }

    std::cout << ans << '\n';
    return 0;
}