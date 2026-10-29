#include <iostream>
#include <array>
#include <vector>
#include <string>

struct SAM
{
    int len = 0;
    int link = -1;
    std::array<int, 26> next;
    SAM() { next.fill(-1); }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::string s;
    std::cin >> s;
    int n = s.length();
    std::vector<SAM> st(2 * n + 1);
    int sz = 1;
    int last = 0;
    for (char ch : s)
    {
        int c = ch - 'a';
        int cur = sz++;
        st[cur].len = st[last].len + 1;
        int ptr = last;
        while (ptr != -1 && st[ptr].next[c] == -1)
        {
            st[ptr].next[c] = cur;
            ptr = st[ptr].link;
        }
        if (ptr == -1)
            st[cur].link = 0;
        else
        {
            int q = st[ptr].next[c];
            if (st[q].len == st[ptr].len + 1)
            {
                st[cur].link = q;
            }
            else
            {
                int clone = sz++;
                st[clone] = st[q];
                st[clone].len = st[ptr].len + 1;
                while (ptr != -1 && st[ptr].next[c] == q)
                {
                    st[ptr].next[c] = clone;
                    ptr = st[ptr].link;
                }
                st[q].link = clone;
                st[cur].link = clone;
            }
        }
        last = cur;
    }

    std::vector<int> diff(n + 2, 0);
    for (int v = 1; v < sz; ++v)
    {
        int left = st[st[v].link].len + 1;
        int right = st[v].len;
        diff[left]++;
        diff[right + 1]--;
    }
    int pref = 0;
    for (int len = 1; len <= n; ++len)
    {
        pref += diff[len];
        std::cout << pref << " ";
    }
    return 0;
}