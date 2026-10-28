#include <vector>
#include <cmath>
#include <iostream>

typedef long long int64;

const int LIMIT = 2e6 + 2;

bool perfect_square(int64 n)
{
    int64 s = std::sqrt(n);
    while (s * s < n)
        ++s;
    while (s * s > n)
        --s;
    return s * s == n;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int t;
    std::cin >> t;
    std::vector<char> composite(LIMIT, 0);
    std::vector<int> primes;
    for (int i = 2; i < LIMIT; ++i)
    {
        if (!composite[i])
        {
            primes.push_back(i);
            for (int64 j = (int64)i * i; j < LIMIT; j += (int64)i)
                composite[j] = 1;
        }
    }
    while (t--)
    {
        int64 n;
        std::cin >> n;
        if (perfect_square(8 * n + 1))
        {
            std::cout << 1 << '\n';
            continue;
        }
        int64 x = 4 * n + 1;
        bool two = true;
        for (int p : primes)
        {
            if ((int64)p * p > x)
                break;
            if (x % p != 0)
                continue;
            int exp = 0;
            while (x % p == 0)
            {
                ++exp;
                x /= p;
            }
            if (p % 4 == 3 && exp % 2 == 1)
            {
                two = false;
                break;
            }
        }
        if (two && x > 1 && x % 4 == 3)
            two = false;
        if (two)
            std::cout << 2 << '\n';
        else
            std::cout << 3 << '\n';
    }
    return 0;
}