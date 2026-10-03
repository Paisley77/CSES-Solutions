#include <iostream>
#include <cstdint>
#include <vector>
#include <string>

const int MAXN = 3000;
int bit[MAXN + 1];
int head[MAXN];
int next[MAXN];

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, k;
    std::cin >> n >> k;
    std::vector<std::string> grid(n);
    for (auto &s : grid)
        std::cin >> s;

    int N = n * n;
    std::vector<uint16_t> A(N), B(N);
    std::vector<uint16_t> up(n, 0), down(n, 0);
    for (int i = n - 1; i >= 0; --i)
    {
        uint16_t left = 0;
        for (int j = 0; j < n; ++j)
        {
            left = (j > 0 && grid[i][j] == grid[i][j - 1]) ? left + 1 : 1;
            down[j] = (i < n - 1 && grid[i][j] == grid[i + 1][j]) ? down[j] + 1 : 1;
            size_t id = (size_t)i * n + j;
            A[id] = std::min(left, down[j]);
        }
    }

    for (int i = 0; i < n; ++i)
    {
        uint16_t right = 0;
        for (int j = n - 1; j >= 0; --j)
        {
            right = (j < n - 1 && grid[i][j] == grid[i][j + 1]) ? right + 1 : 1;
            up[j] = (i > 0 && grid[i][j] == grid[i - 1][j]) ? up[j] + 1 : 1;
            size_t id = (size_t)i * n + j;
            B[id] = std::min(right, up[j]);
        }
    }

    std::vector<long long> ans(k, 0);
    for (int s = 0; s <= 2 * n - 2; ++s)
    {
        int start_r = std::max(s - n + 1, 0), end_r = std::min(s, n - 1);
        int m = end_r - start_r + 1;

        auto add = [&](int idx, int delta = 1)
        {
            for (int i = idx; i <= m; i += (i & (-i)))
                bit[i] += delta;
        };

        auto query = [&](int idx)
        {
            int sum = 0;
            for (int i = idx; i > 0; i -= (i & (-i)))
                sum += bit[i];
            return sum;
        };

        std::fill(bit, bit + m + 1, 0);
        std::fill(head, head + m, -1);
        int expired = 0;
        for (int j = 0; j < m; ++j)
        {
            for (int x = head[j]; x != -1; x = next[x])
            {
                add(x + 1);
                expired++;
            }
            int r = start_r + j, c = s - r;
            size_t id = (size_t)r * n + c;
            int min_x = j - (int)B[id] + 1;
            ans[grid[r][c] - 'A'] += ((int)B[id] - (expired - query(min_x)));
            int max_r = j + (int)A[id];
            if (max_r < m)
            {
                next[j] = head[max_r];
                head[max_r] = j;
            }
        }
    }

    for (int i = 0; i < k; ++i)
        std::cout << ans[i] << '\n';
    return 0;
}