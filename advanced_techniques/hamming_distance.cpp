#include <iostream>
#include <vector>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, k;
    std::cin >> n >> k;
    std::vector<unsigned int> value(n);
    for (int i = 0; i < n; ++i)
    {
        std::string s;
        std::cin >> s;
        unsigned int v = 0;
        for (char c : s)
        {
            v = (v << 1) | (c - '0');
        }
        value[i] = v;
    }

    int ans = k;
    for (int i = 0; i < (n - 1) && ans > 0; ++i)
    {
        for (int j = i + 1; j < n && ans > 0; ++j)
        {
            int d = __builtin_popcount(value[i] ^ value[j]);
            if (d < ans)
                ans = d;
        }
    }

    std::cout << ans << std::endl;
    return 0;
}