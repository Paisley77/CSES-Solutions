#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    std::vector<long long> apple(n);
    long long global_sum = 0;
    for (auto &a : apple)
    {
        std::cin >> a;
        global_sum += a;
    }
    long long ans = global_sum;
    for (int mask = 0; mask < (1 << n); ++mask)
    {
        long long local_sum = 0;
        for (int i = 0; i < n; ++i)
            if ((mask >> i) & 1)
                local_sum += apple[i];
        ans = std::min(ans, llabs(2 * local_sum - global_sum));
    }
    std::cout << ans << '\n';
    return 0;
}