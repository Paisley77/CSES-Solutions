#include <iostream>
#include <cstdint>
#include <vector>
#include <algorithm>

class Fenwick
{
public:
    explicit Fenwick(int n) : bit(n + 1, 0) {}
    void add(int i, int val)
    {
        for (; i < (int)bit.size(); i += (i & -i))
            bit[i] += val;
    }
    int interval_query(int lo, int hi)
    {
        return query(hi) - query(lo - 1);
    }

private:
    std::vector<int> bit;
    int query(int i)
    {
        int count = 0;
        for (; i > 0; i -= (i & -i))
            count += bit[i];
        return count;
    }
};

struct Event
{
    int id;
    int a, b;
    int threshold;
    int8_t sign;
    bool operator<(const Event &other) const
    {
        return threshold < other.threshold;
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, q;
    std::cin >> n >> q;
    std::vector<std::pair<int, int>> values(n);
    for (int i = 0; i < n; ++i)
    {
        int x;
        std::cin >> x;
        values[i] = {x, i + 1};
    }
    std::sort(values.begin(), values.end());
    std::vector<Event> events;
    events.reserve(2 * q);
    for (int i = 0; i < q; ++i)
    {
        int a, b, c, d;
        std::cin >> a >> b >> c >> d;
        events.push_back({i, a, b, c - 1, -1});
        events.push_back({i, a, b, d, 1});
    }
    std::sort(events.begin(), events.end());
    Fenwick fenwick(n + 1);
    std::vector<int> ans(q, 0);
    int ptr = 0;
    for (const Event &evt : events)
    {
        while (ptr < n && values[ptr].first <= evt.threshold)
        {
            fenwick.add(values[ptr].second, 1);
            ++ptr;
        }
        ans[evt.id] += evt.sign * fenwick.interval_query(evt.a, evt.b);
    }
    for (int i = 0; i < q; ++i)
        std::cout << ans[i] << " \n"[i == q - 1];
    return 0;
}