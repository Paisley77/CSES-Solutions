#include <iostream>
#include <string>
#include <algorithm>

const int INF = 1e6 + 1;

int cost(int x, int y, int limit)
{
    int steps = 0;
    while (x != y)
    {

        if (x > y)
        {
            // if gcd(x, y) = 1 and y != 1, then same as x / y; after operation, either x = y = 1 or gcd(x, y) = 1
            // if gcd(x, y) != 1, then after operation either x = y != 1 or still gcd(x,y) != 1
            int q = (x - 1) / y;
            x -= q * y;
            steps += q;
        }
        else
        {
            int q = (y - 1) / x;
            y -= q * x;
            steps += q;
        }
        if (steps >= limit)
            return INF;
    }

    if (x != 1)
        return INF;

    return steps;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    int sum = n + 2;
    int best = n, best_x = 1, best_y = n + 1;
    for (int x = 1; x <= sum / 2; ++x)
    {
        int y = sum - x;
        int cur = cost(x, y, best);
        if (cur < best)
        {
            best_x = x;
            best_y = y;
            best = cur;
        }
    }
    std::string ans = "";
    int x = best_x, y = best_y;
    while (x != y)
    {
        if (x > y)
        {
            int q = (x - 1) / y;
            ans.append(q, '0');
            x -= q * y;
        }
        else
        {
            int q = (y - 1) / x;
            ans.append(q, '1');
            y -= q * x;
        }
    }
    std::reverse(ans.begin(), ans.end());
    std::cout << ans << '\n';
    return 0;
}