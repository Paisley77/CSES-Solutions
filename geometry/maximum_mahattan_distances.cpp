#include <iostream>
#include <algorithm>

typedef long long int64;

const int64 MAX = 2e9;
const int64 MIN = -MAX;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    int64 ans = 0;
    int64 maxsum = MIN, minsum = MAX, maxdiff = MIN, mindiff = MAX;
    while (n--)
    {
        int64 x, y;
        std::cin >> x >> y;
        maxsum = std::max(x + y, maxsum);
        minsum = std::min(x + y, minsum);
        maxdiff = std::max(x - y, maxdiff);
        mindiff = std::min(x - y, mindiff);
        ans = std::max(maxsum - minsum, maxdiff - mindiff);
        std::cout << ans << '\n';
    }
    return 0;
}