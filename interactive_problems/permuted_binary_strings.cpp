#include <string>
#include <vector>
#include <iostream>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    int bits = 0;
    while ((1 << bits) < n)
        bits++;
    std::vector<int> ans(n, 0);
    for (int bit = 0; bit < bits; ++bit)
    {
        std::string query(n, '0');
        for (int i = 1; i < n; ++i)
        {
            if ((i >> bit) & 1)
                query[i] = '1';
        }
        std::cout << "? " << query << std::endl;
        std::string response;
        std::cin >> response;
        for (int i = 0; i < n; ++i)
        {
            if (response[i] == '1')
                ans[i] |= (1 << bit);
        }
    }
    std::cout << "! ";
    for (int i = 0; i < n; ++i)
        std::cout << ans[i] + 1 << " \n"[i == n - 1];
    return 0;
}