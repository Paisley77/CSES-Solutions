#include <iostream>
#include <vector>
#include <map>
#include <set>

class SegmentTree
{
public:
    SegmentTree(int n) : n(n) { tree.resize(4 * n); }
    void build(const std::vector<int> &arr) { build(1, 1, n, arr); }
    void update(int pos, int val) { update(1, 1, n, pos, val); }
    int query(int ql, int qr) { return query(1, 1, n, ql, qr); }

private:
    int n;
    std::vector<int> tree;
    void build(int node, int l, int r, const std::vector<int> &arr)
    {
        if (l == r)
        {
            tree[node] = arr[l];
            return;
        }
        int mid = (l + r) >> 1;
        build(2 * node, l, mid, arr);
        build(2 * node + 1, mid + 1, r, arr);
        tree[node] = std::max(tree[2 * node], tree[2 * node + 1]);
    }
    void update(int node, int l, int r, int pos, int val)
    {
        if (l == r)
        {
            tree[node] = val;
            return;
        }
        int mid = (l + r) >> 1;
        if (pos <= mid)
            update(2 * node, l, mid, pos, val);
        else
            update(2 * node + 1, mid + 1, r, pos, val);
        tree[node] = std::max(tree[2 * node], tree[2 * node + 1]);
    }
    int query(int node, int l, int r, int ql, int qr)
    {
        if (ql > r || qr < l)
            return 0;
        if (ql <= l && qr >= r)
            return tree[node];
        int mid = (l + r) >> 1;
        return std::max(query(2 * node, l, mid, ql, qr), query(2 * node + 1, mid + 1, r, ql, qr));
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, q;
    std::cin >> n >> q;
    std::vector<int> x(n + 1, 0);
    std::map<int, std::set<int>> val2Indices;
    std::vector<int> prev(n + 1, 0);
    for (int i = 1; i <= n; ++i)
    {
        std::cin >> x[i];
        auto &cur = val2Indices[x[i]];
        if (!cur.empty())
            prev[i] = *(cur.rbegin());
        cur.insert(i);
    }
    SegmentTree seg(n);
    seg.build(prev);
    while (q--)
    {
        int type;
        std::cin >> type;
        if (type == 1)
        {
            int k, u;
            std::cin >> k >> u;
            int old = x[k];
            if (u == old)
                continue;
            auto &oldSet = val2Indices[old];
            auto it = oldSet.find(k);
            auto nextIt = std::next(it);
            if (nextIt != oldSet.end())
            {
                int s = *nextIt;
                prev[s] = prev[k];
                seg.update(s, prev[k]);
            }
            oldSet.erase(it);
            auto &newSet = val2Indices[u];
            nextIt = newSet.lower_bound(k);
            int p = 0;
            if (nextIt != newSet.begin())
            {
                auto prevIt = std::prev(nextIt);
                p = *prevIt;
            }
            prev[k] = p;
            seg.update(k, p);
            if (nextIt != newSet.end())
            {
                int s = *nextIt;
                prev[s] = k;
                seg.update(s, k);
            }
            newSet.insert(k);
            x[k] = u;
        }
        else
        {
            int l, r;
            std::cin >> l >> r;
            if (seg.query(l, r) < l)
                std::cout << "YES\n";
            else
                std::cout << "NO\n";
        }
    }
    return 0;
}