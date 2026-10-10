#include <iostream>
#include <vector>

typedef long long int64;

struct Line
{
    int64 m = 0;
    int64 b = 0;
    int64 eval(int64 x) const { return m * x + b; }
};

class LiChaoSegmentTree
{
public:
    explicit LiChaoSegmentTree(int sz) : sz(sz), tree(4 * sz) {}

    void add_segment(int ql, int qr, Line segment)
    {
        ql = std::max(0, ql);
        qr = std::min(sz - 1, qr);
        if (ql <= qr)
            add_segment(1, 0, sz - 1, ql, qr, segment);
    }

    int64 query(int64 x) const
    {
        return query(1, 0, sz - 1, x);
    }

private:
    // tree[p] stores the line that evaluates best at the midpoint of [L, R] in the current subtree
    // Every line segment is stored in the highest nodes where they evaluate best in the midpoint
    int sz;
    std::vector<Line> tree;

    void add_line(int p, int l, int r, Line nw)
    {
        int mid = (l + r) >> 1;
        bool better_mid = nw.eval(mid) > tree[p].eval(mid);
        bool better_left = nw.eval(l) > tree[p].eval(l);
        if (better_mid)
            std::swap(nw, tree[p]);
        if (l == r)
            return;
        if (better_mid != better_left)
        {
            add_line(2 * p, l, mid, nw);
            return;
        }
        bool better_right = nw.eval(r) > tree[p].eval(r);
        if (better_right)
            add_line(2 * p + 1, mid + 1, r, nw);
    }

    void add_segment(int p, int l, int r, int ql, int qr, Line segment)
    {
        if (qr < l || ql > r)
            return;
        if (ql <= l && qr >= r)
        {
            add_line(p, l, r, segment);
            return;
        }
        int mid = (l + r) >> 1;
        add_segment(2 * p, l, mid, ql, qr, segment);
        add_segment(2 * p + 1, mid + 1, r, ql, qr, segment);
    }

    int64 query(int p, int l, int r, int64 x) const
    {
        // lines not queried are either:
        // 1) not defined on x or,
        // 2) guaranteed not to be best at some interval that cover x
        int64 res = tree[p].eval(x);
        if (l == r)
            return res;
        int mid = (l + r) >> 1;
        if (x <= mid)
            return std::max(res, query(2 * p, l, mid, x));
        return std::max(res, query(2 * p + 1, mid + 1, r, x));
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, k;
    std::cin >> n >> k;
    std::vector<int64> H(n);
    for (auto &x : H)
        std::cin >> x;
    std::vector<int> L(n), R(n);
    {
        std::vector<int> stk;
        stk.reserve(n);
        for (int i = 0; i < n; ++i)
        {
            while (!stk.empty() && H[stk.back()] >= H[i])
                stk.pop_back();
            L[i] = stk.empty() ? 0 : stk.back() + 1;
            stk.push_back(i);
        }
        stk.clear();
        for (int i = n - 1; i >= 0; --i)
        {
            while (!stk.empty() && H[stk.back()] >= H[i])
                stk.pop_back();
            R[i] = stk.empty() ? n - 1 : stk.back() - 1;
            stk.push_back(i);
        }
    }

    LiChaoSegmentTree cht(n - k + 1);
    for (int i = 0; i < n; ++i)
    {
        int l = L[i], r = R[i];
        int64 h = H[i];
        int p = std::min(l, r - k + 1);
        int q = std::max(l, r - k + 1);
        cht.add_segment(l - k + 1, p, Line{h, h * (k - l)});
        cht.add_segment(p, q, Line{0, h * std::min(r - l + 1, k)});
        cht.add_segment(q, r, Line{-h, h * (r + 1)});
    }
    for (int x = 0; x <= n - k; ++x)
        std::cout << cht.query(x) << " \n"[x == n - k];
    return 0;
}