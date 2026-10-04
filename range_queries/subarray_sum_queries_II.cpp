#include <iostream>
#include <vector>
#include <algorithm>

typedef long long ll;

struct Tree
{
    ll sum;
    ll prefix;
    ll suffix;
    ll best;
};

Tree merge(const Tree &L, const Tree &R)
{
    ll sum = L.sum + R.sum;
    ll prefix = std::max(L.prefix, L.sum + R.prefix);
    ll suffix = std::max(R.suffix, R.sum + L.suffix);
    ll best = std::max({L.best, R.best, L.suffix + R.prefix});
    return Tree{sum, prefix, suffix, best};
}

class SegmentTree
{
private:
    std::vector<Tree> tree;

    void build(int p, int left, int right, const std::vector<int> &arr)
    {
        if (left == right)
        {
            int x = arr[left - 1];
            tree[p] = {x, std::max(x, 0), std::max(x, 0), std::max(x, 0)};
            return;
        }
        int mid = (left + right) >> 1;
        build(2 * p, left, mid, arr);
        build(2 * p + 1, mid + 1, right, arr);
        tree[p] = merge(tree[2 * p], tree[2 * p + 1]);
    }

    Tree query(int p, int left, int right, int a, int b)
    {
        if (left >= a && right <= b)
            return tree[p];
        int mid = (left + right) >> 1;
        if (mid >= b)
            return query(2 * p, left, mid, a, b);
        if (mid < a)
            return query(2 * p + 1, mid + 1, right, a, b);
        return merge(query(2 * p, left, mid, a, b), query(2 * p + 1, mid + 1, right, a, b));
    }

public:
    int n;

    SegmentTree(const std::vector<int> &arr)
    {
        n = arr.size();
        tree.resize(4 * n);
        build(1, 1, n, arr);
    }

    ll query_interval(int a, int b) { return query(1, 1, n, a, b).best; }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, q;
    std::cin >> n >> q;
    std::vector<int> arr(n);
    for (auto &x : arr)
        std::cin >> x;
    SegmentTree seg_tree(arr);
    while (q--)
    {
        int a, b;
        std::cin >> a >> b;
        std::cout << seg_tree.query_interval(a, b) << '\n';
    }
    return 0;
}