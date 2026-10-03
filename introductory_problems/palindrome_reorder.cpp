#include <iostream>
#include <string>
#include <array>
#include <algorithm>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::string s;
    std::cin >> s;
    std::array<int, 26> cnt{};
    for (char &c : s)
    {
        cnt[c - 'A']++;
    }
    int odd = -1;
    for (int i = 0; i < 26; ++i)
    {
        if (cnt[i] % 2 == 1)
        {
            if (odd != -1)
            {
                std::cout << "NO SOLUTION" << '\n';
                return 0;
            }
            odd = i;
        }
    }
    std::string left = "";
    for (int i = 0; i < 26; ++i)
    {
        left.append(cnt[i] / 2, char('A' + i));
    }
    std::cout << left;
    if (odd != -1)
        std::cout << char('A' + odd);
    std::reverse(left.begin(), left.end());
    std::cout << left << '\n';
    return 0;
}