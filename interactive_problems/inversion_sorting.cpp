#include <iostream>
#include <vector>
#include <algorithm>

int ask(int i, int j)
{
    std::cout << i << " " << j << '\n';
    std::cout.flush();
    int numInv;
    std::cin >> numInv;
    return numInv;
}

int main()
{
    int n;
    std::cin >> n;
    std::vector<int> rank(n + 1);
    rank[1] = 1;
    int curInv = ask(1, 1), invPrev = 0;
    for (int k = 2; k <= n; ++k)
    {
        int newInv = ask(1, k);
        int C = k * (k - 1) / 2;
        int invIn = (curInv - newInv + C) / 2;
        int S = invIn - invPrev;
        int r = k - S;
        for (int i = 1; i < k; ++i)
            if (rank[i] >= r)
                rank[i]++;
        rank[k] = r;
        std::reverse(rank.begin() + 1, rank.begin() + k + 1);
        invPrev = C - invIn;
        curInv = newInv;
    }

    for (int i = 1; i <= n; ++i)
    {
        if (rank[i] == i)
            continue;
        int j = -1;
        for (int p = i + 1; p <= n; ++p)
        {
            if (rank[p] == i)
            {
                j = p;
                break;
            }
        }
        int newInv = ask(i, j);
        std::reverse(rank.begin() + i, rank.begin() + j + 1);
        if (newInv == 0)
            return 0;
    }

    return 0;
}