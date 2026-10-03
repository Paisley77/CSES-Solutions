#include <iostream>
#include <algorithm>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    long long sum = 0, mx = 0;
    for (int i = 0; i < n; ++i)
    {
        long long t;
        std::cin >> t;
        sum += t;
        mx = std::max(mx, t);
    }

    std::cout << std::max(2 * mx, sum) << '\n';
    return 0;
}