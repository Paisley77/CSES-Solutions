#include <iostream>
#include <vector>
#include <algorithm>

const int INF = 2e6 + 1;

struct Point
{
    int x;
    int y;
};

struct Query
{
    int x;
    int y;
    int id;
};

class Fenwick
{
public:
    std::vector<int> bit;
    Fenwick(int sz) { bit.assign(sz + 1, -INF); }

    void update(int idx, int val)
    {
        for (; idx < (int)bit.size(); idx += idx & (-idx))
            bit[idx] = std::max(bit[idx], val);
    }

    int prefix(int idx)
    {
        int res = -INF;
        for (; idx > 0; idx -= idx & (-idx))
            res = std::max(res, bit[idx]);
        return res;
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    std::vector<Point> reserved(n), free(m);
    for (int i = 0; i < n; ++i)
        std::cin >> reserved[i].x >> reserved[i].y;
    for (int i = 0; i < m; ++i)
        std::cin >> free[i].x >> free[i].y;

    std::vector<int> nearest(m, INF);
    std::vector<int> signs = {-1, 1};
    for (int sx : signs)
    {
        for (int sy : signs)
        {
            std::vector<Point> r(n);
            std::vector<Query> q(m);
            std::vector<int> ys;
            ys.reserve(n + m);
            for (int i = 0; i < n; ++i)
            {
                r[i].x = reserved[i].x * sx;
                r[i].y = reserved[i].y * sy;
                ys.push_back(r[i].y);
            }
            for (int i = 0; i < m; ++i)
            {
                q[i].x = free[i].x * sx;
                q[i].y = free[i].y * sy;
                q[i].id = i;
                ys.push_back(q[i].y);
            }
            std::sort(ys.begin(), ys.end());
            ys.erase(std::unique(ys.begin(), ys.end()), ys.end());
            std::sort(r.begin(), r.end(), [](const Point &a, const Point &b)
                      { return a.x < b.x; });
            std::sort(q.begin(), q.end(), [](const Query &a, const Query &b)
                      { return a.x < b.x; });
            Fenwick fenwick(ys.size());
            int ptr = 0;
            for (const Query &cur : q)
            {
                while (ptr < n && r[ptr].x <= cur.x)
                {
                    int pos = std::lower_bound(ys.begin(), ys.end(), r[ptr].y) - ys.begin() + 1;
                    fenwick.update(pos, r[ptr].x + r[ptr].y);
                    ptr++;
                }
                int pos = std::upper_bound(ys.begin(), ys.end(), cur.y) - ys.begin();
                int qdrant_best = fenwick.prefix(pos);
                if (qdrant_best != -INF)
                    nearest[cur.id] = std::min(cur.x + cur.y - qdrant_best, nearest[cur.id]);
            }
        }
    }

    int ans = -INF;
    for (int i = 0; i < m; ++i)
        ans = std::max(ans, nearest[i]);

    std::cout << ans << '\n';
    return 0;
}
