#include <iostream>
#include <vector>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<long long> cnt(n, 0);
    cnt[0] = 1;

    long long prefix = 0;
    for (int i = 0; i < n; ++i)
    {
        long long x;
        std::cin >> x;
        prefix += x;
        int r = static_cast<int>(((prefix % n) + n) % n);
        ++cnt[r];
    }

    long long ans = 0;
    for (int k = 0; k < n; ++k)
    {
        ans += cnt[k] * (cnt[k] - 1) / 2;
    }

    std::cout << ans << '\n';
    return 0;
}