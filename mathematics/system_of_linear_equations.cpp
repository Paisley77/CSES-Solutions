#include <iostream>
#include <vector>

const int MOD = 1e9 + 7;

long long mod_pow(long long a, long long e)
{
    long long res = 1;
    while (e != 0)
    {
        if (e & 1)
            res = res * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return res;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<long long>> a(n, std::vector<long long>(m + 1));
    for (int i = 0; i < n; ++i)
    {
        for (auto &e : a[i])
            std::cin >> e;
    }

    std::vector<int> pivot_col;
    pivot_col.reserve(std::min(n, m));
    int row = 0;
    for (int col = 0; col < m && row < n; ++col)
    {
        int pivot = -1;
        for (int i = row; i < n; ++i)
        {
            if (a[i][col] != 0)
            {
                pivot = i;
                break;
            }
        }
        if (pivot == -1)
            continue;
        std::swap(a[row], a[pivot]);
        long long inv_pivot = mod_pow(a[row][col], MOD - 2);
        for (int j = col; j <= m; ++j)
            a[row][j] = a[row][j] * inv_pivot % MOD;
        for (int i = row + 1; i < n; ++i)
        {
            long long factor = a[i][col];
            if (factor == 0)
                continue;
            for (int j = col; j <= m; ++j)
            {
                a[i][j] = (a[i][j] - a[row][j] * factor % MOD + MOD) % MOD;
            }
        }
        pivot_col.push_back(col);
        row++;
    }

    int rank = row;
    for (int i = rank; i < n; ++i)
    {
        if (a[i][m] != 0)
        {
            std::cout << -1 << '\n';
            return 0;
        }
    }

    std::vector<long long> x(m, 0);
    for (int i = rank - 1; i >= 0; --i)
    {
        long long rightmost = a[i][m];
        int col = pivot_col[i];
        for (int j = col + 1; j < m; ++j)
        {
            rightmost = (rightmost - a[i][j] * x[j] % MOD + MOD) % MOD;
        }
        x[col] = rightmost;
    }

    for (int i = 0; i < m; ++i)
        std::cout << x[i] << " \n"[i == m - 1];

    return 0;
}