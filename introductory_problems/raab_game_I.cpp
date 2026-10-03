#include <iostream>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int T;
    std::cin >> T;
    while (T--)
    {
        int n, a, b;
        std::cin >> n >> a >> b;
        if (a + b > n || (a == 0) != (b == 0))
        {
            std::cout << "NO" << '\n';
            continue;
        }
        std::cout << "YES" << '\n';
        for (int i = 1; i <= n; ++i)
            std::cout << i << ' ';
        std::cout << '\n';
        int m = a + b;
        for (int i = 1; i <= b; ++i)
            std::cout << a + i << ' ';
        for (int i = 1; i <= a; ++i)
            std::cout << i << ' ';
        for (int i = m + 1; i <= n; ++i)
            std::cout << i << ' ';
        std::cout << '\n';
    }
    return 0;
}