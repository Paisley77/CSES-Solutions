#include <iostream>
#include <vector>
#include <string>

int fail()
{
    std::cout << "-1\n";
    return 0;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::string s;
    std::cin >> s;
    int n = s.length();
    std::vector<int> cnt(26, 0);
    for (char &c : s)
        cnt[int(c - 'A')]++;
    for (int c = 0; c < 26; ++c)
        if (cnt[c] > (n + 1) / 2)
            return fail();
    int last = -1;
    std::string ans;
    ans.reserve(n);
    for (int L = n; L >= 1; --L)
    {
        int pick = -1;
        if (L & 1)
        {
            int ceil = (L + 1) / 2;
            for (int c = 0; c < 26; ++c)
            {
                if (cnt[c] == ceil)
                {
                    pick = c;
                    break; // there can be at most one such char
                }
            }
        }
        if (pick != -1)
        {
            if (pick == last)
                return fail();
        }
        else // L odd and no forced choice, or L even
        {
            for (int c = 0; c < 26; ++c)
            {
                if (c != last && cnt[c] > 0)
                {
                    pick = c;
                    break; // smallest remaining char other than last
                }
            }
            if (pick == -1)
                return fail();
        }
        ans += char('A' + pick);
        --cnt[pick];
        last = pick;
    }
    std::cout << ans << '\n';
    return 0;
}