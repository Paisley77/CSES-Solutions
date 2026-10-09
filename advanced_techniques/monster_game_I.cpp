#include <iostream>
#include <deque>
#include <vector>

typedef long long int64;

struct Line
{
    int64 m;
    int64 b;
    int64 value(int64 x) const { return m * x + b; }
};

bool redundant(const Line &a, const Line &b, const Line &c)
{
    // m_a > m_b > m_c
    return (__int128)(b.b - a.b) * (b.m - c.m) >= (__int128)(c.b - b.b) * (a.m - b.m);
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::deque<Line> hull;
    auto add_line = [&](int64 m, int64 b)
    {
        while (!hull.empty() && hull.back().m == m)
        {
            if (hull.back().b <= b)
                return;
            hull.pop_back();
        }
        Line cur{m, b};
        // B valid for A ==> B valid for all predecessor lines ==> (induction) ==> ALl middle lines valid
        while (hull.size() >= 2 && redundant(hull[hull.size() - 2], hull.back(), cur))
            hull.pop_back();
        hull.push_back(cur);
    };

    auto query = [&](int64 x)
    {
        while (hull.size() >= 2 && hull[0].value(x) >= hull[1].value(x))
            hull.pop_front();
        return hull.front().value(x);
    };

    int n;
    int64 x;
    std::cin >> n >> x;
    std::vector<int64> s(n), f(n);
    for (auto &p : s)
        std::cin >> p;
    for (auto &p : f)
        std::cin >> p;

    hull.push_back({x, 0});
    int64 dp = 0; // mininum time in total after killing monster i
    for (int i = 0; i < n; ++i)
    {
        dp = query(s[i]);
        add_line(f[i], dp);
    }

    std::cout << dp << '\n';
    return 0;
}