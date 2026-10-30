#include <iostream>
#include <algorithm>
#include <vector>

typedef long long int64;

const int64 INF = 9e15;

int N;
std::vector<int64> prev, cur;
std::vector<int64> cost;

int64 get_cost(int l, int r) { return cost[(size_t)r * N + l]; }

void divide_conquer(int left, int right, int optLeft, int optRight, int g)
{
    if (left > right)
        return;
    int mid = (left + right) >> 1;
    int from = std::max(optLeft, g - 1);
    int to = std::min(optRight, mid - 1);
    int64 best = INF;
    int bestPos = -1;
    for (int p = from; p <= to; ++p)
    {
        int64 cand = prev[p] + get_cost(p + 1, mid);
        if (cand < best)
        {
            best = cand;
            bestPos = p;
        }
    }
    cur[mid] = best; // dp[g][mid]
    divide_conquer(left, mid - 1, optLeft, bestPos, g);
    divide_conquer(mid + 1, right, bestPos, optRight, g);
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, k;
    std::cin >> n >> k;
    N = n + 1;
    std::vector<int64> W((size_t)N, 0), P((size_t)N, 0);
    for (int i = 1; i <= n; ++i)
    {
        int64 c;
        std::cin >> c;
        W[i] = W[i - 1] + c;
        P[i] = P[i - 1] + (int64)i * c;
    }
    // compute cost(l,r)
    cost.resize((size_t)N * N);
    for (int l = 1; l <= n; ++l)
    {
        int m = l;
        for (int r = l; r <= n; ++r)
        {
            int64 total = W[r] - W[l - 1];
            while (m < r && 2 * (W[m] - W[l - 1]) < total)
                ++m;
            cost[(size_t)r * N + l] = m * (W[m] - W[l - 1]) - (P[m] - P[l - 1]) + (P[r] - P[m]) - m * (W[r] - W[m]);
        }
    }

    prev.assign((size_t)N, INF);
    prev[0] = 0; // zero cost for no school && no house
    cur.resize((size_t)N);
    for (int g = 1; g <= k; ++g)
    {
        divide_conquer(g, n, g - 1, n - 1, g);
        std::swap(prev, cur);
    }
    std::cout << prev[n] << '\n';
    return 0;
}
