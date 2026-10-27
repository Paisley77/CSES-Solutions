#include <iostream>
#include <vector>
#include <string>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    std::vector<int> suffix(n), rank(n + 1, 0);
    for (int i = 0; i < n; ++i)
    {
        std::cin >> suffix[i];
        --suffix[i];
    }
    for (int i = 0; i < n; ++i)
        rank[suffix[i]] = i + 1;
    std::string s(n, '?');
    s[suffix[0]] = 'a';
    int letter = 0;
    for (int i = 1; i < n; ++i)
    {
        int prev = suffix[i - 1];
        int cur = suffix[i];
        if (rank[prev + 1] > rank[cur + 1])
            ++letter;
        if (letter >= 26)
        {
            std::cout << -1 << '\n';
            return 0;
        }
        s[cur] = char('a' + letter);
    }
    std::cout << s << '\n';
    return 0;
}