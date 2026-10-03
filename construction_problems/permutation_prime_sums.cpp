#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<int> primes;
    std::vector<char> isComposite(2 * n, 0);
    for (int i = 2; i < 2 * n; ++i)
    {
        if (!isComposite[i])
        {
            primes.push_back(i);
            for (long long j = (long long)i * i; j < 2 * n; j += i)
                isComposite[j] = 1;
        }
    }

    std::vector<char> used(n + 1, 0);
    std::vector<int> b(n + 1);
    bool found = false;
    for (int x = n; x >= 1; --x)
    {
        if (used[x])
            continue;
        if (x == 1)
        {
            b[1] = 1;
            used[1] = 1;
            break;
        }
        int pos = std::lower_bound(primes.begin(), primes.end(), 2 * x) - primes.begin() - 1;
        found = false;
        for (; pos >= 0 && primes[pos] > x; --pos)
        {
            int y = primes[pos] - x;
            if (!used[y])
            {
                b[x] = y;
                b[y] = x;
                used[x] = used[y] = 1;
                found = true;
                break;
            }
        }
        if (!found)
        {
            std::cout << "IMPOSSIBLE" << std::endl;
            return 0;
        }
    }

    for (int i = 1; i <= n; ++i)
    {
        std::cout << i << " \n"[i == n];
    }
    for (int i = 1; i <= n; ++i)
    {
        std::cout << b[i] << " \n"[i == n];
    }

    return 0;
}