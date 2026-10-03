#include <iostream>
#include <vector>
typedef long long ll;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, k, x, a, b, c;
    std::cin >> n >> k >> x >> a >> b >> c;

    ll cur = x, sum = 0, ans = 0;
    std::vector<int> buf(k);
    for (int i = 0; i < n; ++i)
    {
        if (i >= k)
            sum -= buf[i % k];
        buf[i % k] = (int)cur;
        sum += cur;
        if (i >= (k - 1))
            ans ^= sum;
        cur = (a * cur + b) % c;
    }

    std::cout << ans << std::endl;
    return 0;
}