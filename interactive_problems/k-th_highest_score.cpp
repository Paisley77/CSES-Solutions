#include <vector>
#include <iostream>

const int INF = 1e9 + 1;

int main()
{
    std::ios::sync_with_stdio(false);
    int n, k;
    std::cin >> n >> k;
    std::vector<int> F(n + 1, 0), S(n + 1, 0);
    auto getFScore = [&](int x)
    {
        if (x == 0)
            return INF;
        if (x == n + 1)
            return 0;
        if (F[x] > 0)
            return F[x];
        std::cout << "F " << x << std::endl;
        std::cin >> F[x];
        return F[x];
    };
    auto getSScore = [&](int x)
    {
        if (x == 0)
            return INF;
        if (x == n + 1)
            return 0;
        if (S[x] > 0)
            return S[x];
        std::cout << "S " << x << std::endl;
        std::cin >> S[x];
        return S[x];
    };
    int Fx, Fx1, Sy, Sy1;
    int lo = std::max(0, k - n), hi = std::min(n, k);
    while (true)
    {
        int x = (lo + hi) >> 1;
        Fx = getFScore(x);
        Fx1 = getFScore(x + 1);
        Sy = getSScore(k - x);
        Sy1 = getSScore(k - x + 1);
        if (Fx > Sy1 && Fx1 < Sy)
        {
            std::cout << "! " << std::min(Fx, Sy) << '\n';
            return 0;
        }
        if (Fx < Sy1)
            hi = x - 1;
        if (Fx1 > Sy)
            lo = x + 1;
    }
}