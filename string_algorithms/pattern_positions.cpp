#include <string>
#include <iostream>

const int MAXN = 1e5;
const int NSTATES = 2 * MAXN;

struct State
{
    int len;
    int firstpos;
    int link;
    int next[26];
};

State st[NSTATES];
int last, sz;

void sam_init()
{
    st[0].len = 0;
    st[0].firstpos = 0;
    st[0].link = -1;
    for (int i = 0; i < 26; ++i)
        st[0].next[i] = -1;
    last = 0;
    sz = 1;
}

void sam_extend(char c)
{
    int cur = sz++;
    st[cur].len = st[last].len + 1;
    st[cur].firstpos = st[cur].len;
    for (int i = 0; i < 26; ++i)
        st[cur].next[i] = -1;
    int char_idx = c - 'a';
    int p = last;
    while (p != -1 && st[p].next[char_idx] == -1)
    {
        st[p].next[char_idx] = cur;
        p = st[p].link;
    }
    if (p == -1)
    {
        st[cur].link = 0;
    }
    else
    {
        int q = st[p].next[char_idx];
        if (st[q].len == st[p].len + 1)
        {
            st[cur].link = q;
        }
        else
        {
            int clone = sz++;
            st[clone].len = st[p].len + 1;
            st[clone].firstpos = st[q].firstpos;
            st[clone].link = st[q].link;
            for (int i = 0; i < 26; ++i)
                st[clone].next[i] = st[q].next[i];
            while (p != -1 && st[p].next[char_idx] == q)
            {
                st[p].next[char_idx] = clone;
                p = st[p].link;
            }
            st[cur].link = st[q].link = clone;
        }
    }
    last = cur;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::string s;
    std::cin >> s;
    sam_init();
    for (char c : s)
        sam_extend(c);
    int k;
    std::cin >> k;
    std::string pattern;
    while (k--)
    {
        std::cin >> pattern;
        int cur = 0;
        bool found = true;
        for (char c : pattern)
        {
            int char_idx = c - 'a';
            int p = st[cur].next[char_idx];
            if (p == -1)
            {
                found = false;
                break;
            }
            cur = p;
        }
        if (found)
        {
            int ans = st[cur].firstpos - pattern.size() + 1;
            std::cout << ans << '\n';
        }
        else
        {
            std::cout << -1 << '\n';
        }
    }

    return 0;
}