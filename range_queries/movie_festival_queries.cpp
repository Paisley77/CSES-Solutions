#include <iostream>

const int T = 1e6;
const int SZ = T + 2;
const int INF = T + 1;
const int LOG = 18;

int up[LOG][SZ];

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;

    for (int t = 0; t < SZ; ++t)
    {
        up[0][t] = INF;
    }

    int a, b;
    for (int i = 0; i < n; ++i)
    {
        std::cin >> a >> b;
        up[0][a] = std::min(up[0][a], b);
    }

    for (int t = T; t >= 0; --t)
    {
        up[0][t] = std::min(up[0][t], up[0][t + 1]);
    }

    for (int k = 1; k < LOG; ++k)
    {
        for (int t = 0; t < SZ; ++t)
        {
            up[k][t] = up[k - 1][up[k - 1][t]];
        }
    }

    int t, ans;
    for (int i = 0; i < q; ++i)
    {
        std::cin >> a >> b;
        t = a;
        ans = 0;
        for (int k = LOG - 1; k >= 0; --k)
        {
            if (up[k][t] <= b)
            {
                t = up[k][t];
                ans += (1 << k);
            }
        }
        std::cout << ans << '\n';
    }

    return 0;
}