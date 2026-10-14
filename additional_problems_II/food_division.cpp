// d(i) = a(i) - b(i), net surplus
// f(i) = f(i-1) + d(i), balance eqn.
// Let c = f(n). Then f(i) = c + p(i), where p(i) = d(1) + ... + d(i)
// Total cost = sum of |f(i)| = |c + p(i)|
#include <iostream>
#include <vector>
#include <algorithm>

typedef long long int64;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    std::vector<int> a(n), b(n);
    for (auto &x : a)
        std::cin >> x;
    for (auto &y : b)
        std::cin >> y;
    std::vector<int64> prefix(n);
    int64 sum = 0;
    for (int i = 0; i < n; ++i)
    {
        prefix[i] = sum + b[i] - a[i];
        sum = prefix[i];
    }
    std::sort(prefix.begin(), prefix.end());
    int64 c = prefix[n / 2];
    int64 ans = 0;
    for (int i = 0; i < n; ++i)
        ans += llabs(prefix[i] - c);
    std::cout << ans << '\n';
    return 0;
}
