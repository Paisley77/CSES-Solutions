#include <iostream>
#include <vector>
#include <algorithm>

const int MOD = 1e9 + 7;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    std::vector<int> a(n);
    for (auto &x : a)
        std::cin >> x;
    std::sort(a.begin(), a.end());
    long long ans = 1;
    for (int i = 0; i < n;)
    {
        int j = i;
        while (++j < n && a[i] == a[j])
        {
        }
        int freq = j - i;
        ans = ans * (freq + 1) % MOD;
        i = j;
    }
    ans = (ans - 1 + MOD) % MOD;
    std::cout << ans;
    return 0;
}