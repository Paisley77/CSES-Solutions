#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>

typedef long long ll;

std::vector<ll> tree;

void update(int pos, ll value, int node, int low, int high)
{
    if (low == high)
    {
        tree[node] = value;
        return;
    }
    int mid = (low + high) >> 1;
    if (pos <= mid)
        update(pos, value, 2 * node, low, mid);
    else
        update(pos, value, 2 * node + 1, mid + 1, high);
    tree[node] = std::max(tree[2 * node], tree[2 * node + 1]);
}

ll query(int left, int right, int node, int low, int high)
{
    if (low > right || high < left)
        return 0;
    if (low >= left && high <= right)
        return tree[node];
    int mid = (low + high) >> 1;
    return std::max(query(left, right, 2 * node, low, mid), query(left, right, 2 * node + 1, mid + 1, high));
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<ll> h(n);
    for (auto &x : h)
        std::cin >> x;

    std::vector<int> P(n), Q(n);
    {
        std::vector<int> stk;
        stk.reserve(n);
        for (int i = 0; i < n; ++i)
        {
            while (!stk.empty() && h[stk.back()] < h[i])
                stk.pop_back();
            P[i] = stk.empty() ? -1 : stk.back();
            stk.push_back(i);
        }
    }
    {
        std::vector<int> stk;
        stk.reserve(n);
        for (int i = n - 1; i >= 0; --i)
        {
            while (!stk.empty() && h[stk.back()] < h[i])
                stk.pop_back();
            Q[i] = stk.empty() ? n : stk.back();
            stk.push_back(i);
        }
    }

    std::vector<int> order(n);
    std::iota(order.begin(), order.end(), 0);
    std::sort(order.begin(), order.end(), [&](int a, int b)
              { return h[a] < h[b]; });

    tree.assign(4 * n, 0);
    std::vector<ll> f(n);
    ll ans = 0;
    for (int i : order)
    {
        ll jumps = query(P[i] + 1, Q[i] - 1, 1, 0, n - 1);
        f[i] = 1 + jumps;
        ans = std::max(ans, f[i]);
        update(i, f[i], 1, 0, n - 1);
    }

    std::cout << ans << std::endl;
    return 0;
}