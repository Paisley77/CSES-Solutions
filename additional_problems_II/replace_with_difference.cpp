#include <iostream>
#include <vector>
#include <bitset>

const int MAXN = 1000;
const int MAXV = 1000;
const int MAXS = MAXN * MAXV;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    int sum = 0;
    std::vector<int> x(n + 1);
    for (int i = 1; i <= n; ++i)
    {
        std::cin >> x[i];
        sum += x[i];
    }
    if (sum % 2 == 1)
    {
        std::cout << -1 << '\n';
        return 0;
    }
    std::vector<std::bitset<MAXS + 1>> dp(n + 1, 0);
    dp[0][0] = 1;
    for (int i = 1; i <= n; ++i)
        dp[i] = dp[i - 1] | (dp[i - 1] << x[i]);
    int target = sum / 2;
    if (dp[n][target] == 0)
    {
        std::cout << -1 << '\n';
        return 0;
    }
    std::vector<int> L, R;
    int current = target;
    for (int i = n; i >= 1; --i)
    {
        if (dp[i - 1][current] == 1)
        {
            R.push_back(x[i]);
            continue;
        }
        L.push_back(x[i]);
        current -= x[i];
    }
    while ((int)L.size() + (int)R.size() > 1)
    {
        if (!L.empty() && !R.empty())
        {
            int a = L.back();
            L.pop_back();
            int b = R.back();
            R.pop_back();
            std::cout << a << " " << b << '\n';
            int d = std::abs(a - b);
            if (a >= b)
                L.push_back(d);
            else
                R.push_back(d);
        }
        else
        {
            std::vector<int> &v = (L.empty()) ? R : L;
            std::cout << "0 0\n";
            v.pop_back();
        }
    }
    return 0;
}
