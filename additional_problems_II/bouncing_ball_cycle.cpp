#include <iostream>
#include <numeric>

typedef long long int64;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int t;
    std::cin >> t;
    while (t--)
    {
        int64 n, m;
        std::cin >> n >> m;
        int64 a = n - 1, b = m - 1;
        int64 g = std::gcd(a, b);
        int64 A = a / g, B = b / g;
        int64 steps = 2 * A * b;
        int64 cells = A * B * (g - 1) + (A + 1) * (B + 1) / 2;
        std::cout << steps << " " << cells << '\n';
    }
    return 0;
}