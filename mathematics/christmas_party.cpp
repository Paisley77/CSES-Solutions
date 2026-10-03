#include <iostream>

const long long MOD = 1e9 + 7;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    long long a = 1, b = 0;
    for (int i = 2; i <= n; ++i)
    {
        long long c = (long long)(i - 1) * ((a + b) % MOD) % MOD;
        a = b;
        b = c;
    }

    std::cout << b << '\n';
    return 0;
}