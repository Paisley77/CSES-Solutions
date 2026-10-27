#include <iostream>
#include <vector>

typedef long long int64;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    std::vector<int64> prefix(n + 1, 0);
    for (int i = 1; i <= n; ++i)
    {
        int x;
        std::cin >> x;
        prefix[i] = prefix[i - 1] + x;
    }
    std::vector<int> hull;
    hull.reserve(n + 1);
    hull.push_back(0);
    for (int i = 1; i <= n; ++i)
    {
        while (hull.size() >= 2)
        {
            int a = hull[hull.size() - 2];
            int b = hull.back();
            __int128 left = (__int128)(prefix[b] - prefix[a]) * (i - b);
            __int128 right = (__int128)(prefix[i] - prefix[b]) * (b - a);
            if (left >= right)
                hull.pop_back();
            else
                break;
        }
        int j = hull.back();
        std::cout << i - j << '\n';
        hull.push_back(i);
    }
    return 0;
}