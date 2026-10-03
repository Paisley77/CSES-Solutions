#include <string>
#include <iostream>

const int MAXN = 2e5;

int len_[MAXN + 2], slink[MAXN + 2];
int nxt[MAXN + 2][26];

std::string s;

int getLink(int v, int i)
{
    while ((i - len_[v] - 1) < 0 || s[i - len_[v] - 1] != s[i])
        v = slink[v];
    return v;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::cin >> s;
    int n = static_cast<int>(s.size());

    len_[0] = -1;
    len_[1] = 0;
    slink[0] = 0;
    slink[1] = 0;
    int sz = 2, last = 0;
    for (int i = 0; i < n; ++i)
    {
        int cur = getLink(last, i);
        int c = s[i] - 'a';
        if (nxt[cur][c] == 0)
        {
            int now = sz++;
            len_[now] = len_[cur] + 2;
            slink[now] = (len_[now] == 1) ? 1 : nxt[getLink(slink[cur], i)][c];
            nxt[cur][c] = now;
        }
        last = nxt[cur][c];
        std::cout << len_[last] << " \n"[i == (n - 1)];
    }
    return 0;
}