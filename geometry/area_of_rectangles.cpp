#include <iostream>
#include <vector>
#include <algorithm>

typedef long long int64;

class SegmentTree
{
public:
    SegmentTree(const std::vector<int> &coords)
    {
        ys = coords;
        int n = ys.size();
        cover.assign(4 * n, 0);
        length.assign(4 * n, 0);
    }

    void update(int ql, int qr, int val) { return update(1, 0, (int)ys.size() - 1, ql, qr, val); }

    int64 getLength() { return length[1]; }

private:
    std::vector<char> cover;
    std::vector<int64> length;
    std::vector<int> ys;

    void update(int node, int l, int r, int ql, int qr, int val)
    {
        if (qr <= l || ql >= r)
            return;
        if (l >= ql && r <= qr)
        {
            cover[node] += val; // number of rectangles that currently cover the segment [l, r]
            pull(node, l, r);
            return;
        }
        int mid = (l + r) >> 1;
        update(2 * node, l, mid, ql, qr, val);
        update(2 * node + 1, mid, r, ql, qr, val);
        pull(node, l, r);
    }

    void pull(int node, int l, int r)
    {
        if (cover[node] > 0)
            length[node] = ys[r] - ys[l];
        else if (r - l == 1)
            length[node] = 0;
        else
            length[node] = length[2 * node] + length[2 * node + 1];
    }
};

struct Event
{
    int x;
    int y1;
    int y2;
    int type;

    bool operator<(const Event &other) const { return x < other.x; }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    std::vector<Event> events;
    events.reserve(2 * n);
    std::vector<int> ys;
    ys.reserve(2 * n);
    for (int i = 0; i < n; ++i)
    {
        int x1, x2, y1, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;
        events.push_back({x1, y1, y2, 1});
        events.push_back({x2, y1, y2, -1});
        ys.push_back(y1);
        ys.push_back(y2);
    }
    std::sort(events.begin(), events.end());
    std::sort(ys.begin(), ys.end());
    ys.erase(std::unique(ys.begin(), ys.end()), ys.end());
    SegmentTree seg(ys);
    int prevX = events[0].x;
    int64 area = 0;
    for (int i = 0; i < (int)events.size();)
    {
        int x = events[i].x;
        area += seg.getLength() * (x - prevX);
        while (i < (int)events.size() && events[i].x == x)
        {
            int y1_pos = std::lower_bound(ys.begin(), ys.end(), events[i].y1) - ys.begin();
            int y2_pos = std::lower_bound(ys.begin(), ys.end(), events[i].y2) - ys.begin();
            seg.update(y1_pos, y2_pos, events[i].type);
            i++;
        }
        prevX = x;
    }
    std::cout << area << '\n';
    return 0;
}