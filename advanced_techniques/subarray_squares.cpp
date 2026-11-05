#include <iostream>
#include <vector>

typedef long long int64;

struct Line
{
    int64 m;
    int64 b;
    int64 eval(int64 x) { return m * x + b; }
};

bool redundant(const Line &A, const Line &B, const Line &C)
{
    // B redundant <=> x_AB >= x_BC
    return (__int128)(A.b - B.b) * (C.m - B.m) >= (__int128)(B.b - C.b) * (B.m - A.m);
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, k;
    std::cin >> n >> k;
    std::vector<int64> pref(n + 1, 0);
    for (int i = 1; i <= n; ++i)
    {
        int64 a;
        std::cin >> a;
        pref[i] = pref[i - 1] + a;
    }
    std::vector<int64> prev(n + 1);
    std::vector<int64> curr(n + 1);
    std::vector<Line> hull(n);
    for (int i = 1; i <= n; ++i)
    {
        // dp[1][i] = pref[i] * pref[i]
        prev[i] = pref[i] * pref[i];
    }
    for (int g = 2; g <= k; ++g)
    {
        int head = 0, tail = 0; // empty hull
        for (int i = g; i <= n; ++i)
        {
            int j = i - 1;
            Line line = {-2 * pref[j], prev[j] + pref[j] * pref[j]};
            while (tail - head >= 2 && redundant(hull[tail - 2], hull[tail - 1], line))
                tail--;
            hull[tail++] = line;
            int64 x = pref[i];
            while (tail - head >= 2 && hull[head].eval(x) >= hull[head + 1].eval(x))
                head++;
            curr[i] = pref[i] * pref[i] + hull[head].eval(x);
        }
        curr.swap(prev);
    }
    std::cout << prev[n] << '\n';
    return 0;
}