#include <vector>
#include <iostream>

class SegmentTree
{
public:
    explicit SegmentTree(int n) : n(n), tree(4 * n) {}

    void addPrefix(int r, int delta)
    {
        update(1, 1, n, 1, r, delta);
    }

    int queryMin() const
    {
        return tree[1].mn;
    }

    int queryMax() const
    {
        return tree[1].mx;
    }

private:
    struct Node
    {
        int mx = 0;
        int mn = 0;
        int lazy = 0;
    };

    int n;
    std::vector<Node> tree;

    void apply(int node, int delta)
    {
        tree[node].mn += delta;
        tree[node].mx += delta;
        tree[node].lazy += delta;
    }

    void push(int node)
    {
        if (tree[node].lazy == 0)
            return;
        int delta = tree[node].lazy;
        apply(2 * node, delta);
        apply(2 * node + 1, delta);
        tree[node].lazy = 0;
    }

    void pull(int node)
    {
        tree[node].mn = std::min(tree[2 * node].mn, tree[2 * node + 1].mn);
        tree[node].mx = std::max(tree[2 * node].mx, tree[2 * node + 1].mx);
    }

    void update(int node, int left, int right, int qleft, int qright, int delta)
    {
        if (qleft <= left && qright >= right)
        {
            apply(node, delta);
            return;
        }
        // Even if, for example, right child does not belong to the interval
        // we still need to propagate delta to properly update right child data
        // so later when we pull, right child data will correctly reflect the min & max
        // of its interval
        push(node);
        int mid = (left + right) >> 1;
        if (qleft <= mid)
            update(2 * node, left, mid, qleft, qright, delta);
        if (qright > mid)
            update(2 * node + 1, mid + 1, right, qleft, qright, delta);
        pull(node); // If the whole interval is not covered, then eventually we pull back to node 1
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    SegmentTree tree(n);
    for (int i = 0; i < n; ++i)
    {
        int c, s;
        std::cin >> c >> s;
        int delta = (s == 1) ? 1 : -1;
        tree.addPrefix(c, delta);
        if (tree.queryMin() >= 0)
            std::cout << ">\n";
        else if (tree.queryMax() <= 0)
            std::cout << "<\n";
        else
            std::cout << "?\n";
    }
    return 0;
}