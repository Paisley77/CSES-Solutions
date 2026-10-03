#include <vector>
#include <algorithm>
#include <iostream>

struct Event
{
    int x, type, y1, y2;
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<Event> evt;
    std::vector<int> ys;

    for (int i = 0; i < n; ++i)
    {
        int x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;

        if (y1 == y2)
        {
            evt.push_back({x1, 2, y1, y2});
            evt.push_back({x2, 0, y1, y2});
            ys.push_back(y1);
        }
        else
        {
            if (y1 > y2)
            {
                std::swap(y1, y2);
            }
            evt.push_back(Event({x1, 1, y1, y2}));
            ys.push_back(y1);
            ys.push_back(y2);
        }
    }

    std::sort(evt.begin(), evt.end(), [](const Event &a, const Event &b)
              {
                  if (a.x != b.x)
                      return a.x < b.x;
                return a.type < b.type; });

    std::sort(ys.begin(), ys.end());
    ys.erase(std::unique(ys.begin(), ys.end()), ys.end());

    const int m = static_cast<int>(ys.size());
    std::vector<int> bit(m + 1, 0);

    auto id = [&](int y)
    { return static_cast<int>(std::lower_bound(ys.begin(), ys.end(), y) - ys.begin() + 1); };

    auto add = [&](int i, int d)
    {
        for (; i <= m; i += i & (-i))
            bit[i] += d;
    };

    auto pref = [&](int i)
    {
        int s = 0;
        for (; i > 0; i -= i & (-i))
            s += bit[i];
        return s;
    };

    long long ans = 0;
    for (const Event &e : evt)
    {
        if (e.type == 0)
        {
            add(id(e.y1), -1);
        }
        else if (e.type == 2)
        {
            add(id(e.y1), +1);
        }
        else
        {
            ans += pref(id(e.y2)) - pref(id(e.y1) - 1);
        }
    }

    std::cout << ans << '\n';
    return 0;
}