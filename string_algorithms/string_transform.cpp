#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

int id(char c)
{
    return (c == '#') ? 0 : (int)(c - 'a' + 1);
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::string s;
    std::cin >> s;
    int n = s.size();
    std::vector<int> cnt(27, 0), rank(n);
    int hash_pos = -1;
    for (int i = 0; i < n; ++i)
    {
        rank[i] = cnt[id(s[i])]++;
        if (s[i] == '#')
            hash_pos = i;
    }
    std::vector<int> prefix(27, 0);
    for (int i = 1; i <= 26; ++i)
        prefix[i] = prefix[i - 1] + cnt[i - 1];
    std::vector<int> LF(n);
    for (int i = 0; i < n; ++i)
        LF[i] = prefix[id(s[i])] + rank[i];
    std::string ans = "";
    ans.reserve(n - 1);
    int pos = hash_pos;
    for (int i = 0; i < n - 1; ++i)
    {
        pos = LF[pos];
        ans.push_back(s[pos]);
    }
    std::reverse(ans.begin(), ans.end());
    std::cout << ans << '\n';
    return 0;
}