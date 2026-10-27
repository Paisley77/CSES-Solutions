#include <iostream>
#include <vector>

class SegmentTree
{
private:
    int n;
    std::vector<int> tree;

public:
    SegmentTree(int n) : n(n) { tree.resize(2 * n); } // tree[1 .. n - 1, n, ..., 2 * n - 1]
    void build(const std::vector<int> &arr)
    {
        for (int i = 0; i < n; ++i)
            tree[i + n] = arr[i];
        for (int i = n - 1; i > 0; --i)
            tree[i] = std::max(tree[2 * i], tree[2 * i + 1]);
    }
    void update(int pos, int val)
    {
        pos += n;
        tree[pos] = val;
        for (int u = pos / 2; u > 0; u = u / 2)
            tree[u] = std::max(tree[2 * u], tree[2 * u + 1]);
    }
    int query(int l, int r)
    {
        l += n;
        r += n;
        int ans = 0;
        while (l <= r)
        { // query range not merged yet
            if (l & 1)
                ans = std::max(ans, tree[l++]);
            if (!(r & 1))
                ans = std::max(ans, tree[r--]);
            l /= 2;
            r /= 2;
        }
        return ans;
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, q;
    std::cin >> n >> q;
    std::vector<int> value(n);
    for (auto &v : value)
        std::cin >> v;
    std::vector<std::vector<int>> graph(n);
    for (int i = 0; i < n - 1; ++i)
    {
        int a, b;
        std::cin >> a >> b;
        graph[--a].push_back(--b);
        graph[b].push_back(a);
    }
    std::vector<int> parent(n, -1), depth(n, 0), subtree(n, 1), heavy(n, -1), head(n), pos(n), flattened(n);
    {
        // Re-order nodes so parents precede children;
        // Fill in parent, depth
        std::vector<int> order, stk;
        order.reserve(n);
        stk.reserve(n);
        stk.push_back(0);
        parent[0] = -2;
        while (!stk.empty())
        {
            int u = stk.back();
            stk.pop_back();
            order.push_back(u);
            for (int v : graph[u])
            {
                if (parent[v] != -1)
                    continue;
                parent[v] = u;
                depth[v] = depth[u] + 1;
                stk.push_back(v);
            }
        }
        parent[0] = -1;

        // fill in subtree & heavy
        for (int idx = n - 1; idx >= 0; --idx)
        {
            int u = order[idx];
            int heavyChild = -1;
            for (int v : graph[u])
            {
                if (parent[v] == u)
                {
                    subtree[u] += subtree[v];
                    if (heavyChild == -1 || subtree[v] > subtree[heavyChild])
                        heavyChild = v;
                }
            }
            heavy[u] = heavyChild; // will be -1 if u is a leaf node
        }
    }

    { // fill in head & pos arrays
        std::vector<int> stk;
        stk.push_back(0);
        int curPos = 0;
        while (!stk.empty())
        {
            int chainHead = stk.back();
            stk.pop_back();
            for (int u = chainHead; u != -1; u = heavy[u])
            {
                head[u] = chainHead;
                pos[u] = curPos++;
                for (int v : graph[u])
                    if (v != parent[u] && v != heavy[u])
                        stk.push_back(v);
            }
        }
    }
    for (int u = 0; u < n; ++u)
        flattened[pos[u]] = value[u];
    SegmentTree seg(n);
    seg.build(flattened);
    auto pathMaximum = [&](int a, int b) { // a and b are node indices (0-indexed)
        int ans = 0;
        while (head[a] != head[b])
        {
            if (depth[head[a]] < depth[head[b]])
                std::swap(a, b);
            ans = std::max(ans, seg.query(pos[head[a]], pos[a]));
            a = parent[head[a]];
        }
        if (depth[a] > depth[b])
            std::swap(a, b);
        ans = std::max(ans, seg.query(pos[a], pos[b]));
        return ans;
    };
    while (q--)
    {
        int type;
        std::cin >> type;
        if (type == 1)
        {
            int s, x;
            std::cin >> s >> x;
            seg.update(pos[--s], x);
        }
        else
        {
            int a, b;
            std::cin >> a >> b;
            std::cout << pathMaximum(--a, --b) << '\n';
        }
    }
    return 0;
}