#include <iostream>

const int MAXN = 1e6;
const long long MOD = 1e9 + 7;

long long A[MAXN + 1];

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    A[0] = 1;
    A[1] = 2;
    A[2] = 8;
    for (int n = 3; n <= MAXN; ++n)
    {
        long long v = (6 * A[n - 1] % MOD - 7 * A[n - 2] % MOD) % MOD;
        A[n] = (v + MOD) % MOD;
    }

    int t, n;
    std::cin >> t;

    while (t--)
    {
        std::cin >> n;
        std::cout << A[n] << '\n';
    }

    return 0;
}