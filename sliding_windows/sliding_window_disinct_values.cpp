#include <vector>
#include <unordered_map>
#include <iostream>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, k;
    std::cin >> n >> k;
    std::vector<int> x(n);
    for (auto &m : x)
        std::cin >> m;

    std::unordered_map<int, int> cnt;
    cnt.reserve(2 * n);
    std::vector<int> distinct(n - k + 1, 0);
    for (int i = 0; i < k; ++i)
        if (cnt[x[i]]++ == 0)
            distinct[0]++;
    for (int i = k; i < n; ++i)
    {
        int in = x[i], out = x[i - k];
        distinct[i - k + 1] = distinct[i - k];
        if (cnt[in]++ == 0)
            distinct[i - k + 1]++;
        if (--cnt[out] == 0)
            distinct[i - k + 1]--;
    }

    for (int val : distinct)
        std::cout << val << '\n';
    return 0;
}