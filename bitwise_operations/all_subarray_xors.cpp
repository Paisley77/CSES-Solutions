#include <vector>
#include <iostream>

typedef long long int64;

const int B = 20;

void fwht(std::vector<int64> &a, bool invert)
{
    int M = (int)a.size();
    for (int len = 1; len < M; len <<= 1)
    {
        for (int i = 0; i < M; i += 2 * len)
        {
            for (int j = 0; j < len; ++j)
            {
                int64 u = a[i + j];
                int64 v = a[i + j + len];
                a[i + j] = u + v;
                a[i + j + len] = u - v;
            }
        }
    }
    if (invert)
    {
        for (int64 &x : a)
            x /= M;
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    int M = 1 << B;
    std::vector<int64> f(M, 0);
    f[0] = 1;
    int prefix = 0;
    for (int i = 1; i <= n; ++i)
    {
        int a;
        std::cin >> a;
        prefix ^= a;
        f[prefix]++;
    }
    fwht(f, false);
    for (int t = 0; t < M; ++t)
        f[t] *= f[t];
    fwht(f, true);
    f[0] -= (n + 1);
    int count = 0;
    for (int k = 0; k < M; ++k)
        if (f[k] > 0)
            count++;
    std::cout << count << '\n';
    for (int k = 0; k < M; ++k)
        if (f[k] > 0)
            std::cout << k << " ";
    return 0;
}