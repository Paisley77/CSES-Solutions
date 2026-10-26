#include <iostream>
#include <vector>
#include <algorithm>

typedef long long int64;

struct Line
{
    int64 m;
    int64 b;
    int64 eval(int64 x) const { return m * x + b; }
    bool operator<(const Line &other) const
    {
        if (m != other.m)
            return m < other.m;
        return b > other.b;
    }
};

bool redundant(const Line &A, const Line &B, const Line &C)
{
    // pre: A.m < B.m < C.m
    return (__int128)((B.b - C.b) * (B.m - A.m)) <= (__int128)((A.b - B.b) * (C.m - B.m));
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, k;
    std::cin >> n >> k;
    std::vector<Line> lines(n);
    for (int i = 0; i < n; ++i)
    {
        int64 y1, y2;
        std::cin >> y1 >> y2;
        lines[i] = {(y2 - y1) / k, y1};
    }
    std::sort(lines.begin(), lines.end());
    std::vector<Line> uniqueLines;
    for (const Line &line : lines)
        if (uniqueLines.empty() || uniqueLines.back().m != line.m)
            uniqueLines.push_back(line);
    std::vector<Line> hull;
    for (const Line &line : lines)
    {
        while (hull.size() >= 2 && redundant(hull[hull.size() - 2], hull[hull.size() - 1], line))
            hull.pop_back();
        hull.push_back(line);
    }
    int ptr = 0;
    for (int64 x = 0; x <= k; ++x)
    {
        while (ptr < (int)hull.size() - 1 && hull[ptr].eval(x) <= hull[ptr + 1].eval(x))
            ptr++;
        std::cout << hull[ptr].eval(x) << '\n';
    }
    return 0;
}