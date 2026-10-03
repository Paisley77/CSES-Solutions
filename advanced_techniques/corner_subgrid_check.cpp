#include <iostream>
#include <vector>
#include <string>
typedef unsigned long long u64;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    int k;
    std::cin >> n >> k;

    std::vector<std::string> grid(n);
    for (auto &s : grid)
        std::cin >> s;

    int word = (n + 63) / 64;
    std::vector<std::vector<u64>> mark(k, std::vector<u64>((size_t)n * word, 0ULL));
    std::vector<std::vector<int>> bucket(k);
    std::vector<char> foundList(k, 0);
    int foundCount = 0;

    for (int row = 0; row < n && foundCount < k; ++row)
    {
        for (int c = 0; c < k; ++c)
            bucket[c].clear();
        const std::string &rowString = grid[row];
        for (int col = 0; col < n; ++col)
        {
            bucket[rowString[col] - 'A'].push_back(col);
        }
        for (int c = 0; c < k; ++c)
        {
            if (foundList[c])
                continue;
            bool found = false;
            auto &L = bucket[c];
            int m = (int)L.size();
            if (m < 2)
                continue;
            u64 *M = mark[c].data();
            for (int a = 0; a < m && !found; ++a)
            {
                int i = L[a];
                u64 *rowPtr = M + (size_t)i * word;
                for (int b = a + 1; b < m; ++b)
                {
                    int j = L[b];
                    u64 bit = 1ULL << (j & 63);
                    if (rowPtr[j >> 6] & bit)
                    {
                        found = true;
                        foundList[c] = 1;
                        foundCount++;
                        break;
                    }
                    rowPtr[j >> 6] |= bit;
                }
            }
        }
    }

    for (int c = 0; c < k; ++c)
    {
        std::cout << (foundList[c] ? "YES" : "NO") << " \n"[c == (k - 1)];
    }

    return 0;
}
