#include <vector>
#include <algorithm>
#include <iostream>

const int MAXNODE = 20 * 2e5; // tree size * n

int tot = 0;
long long reach = 0;
int lc[MAXNODE], rc[MAXNODE];
long long sm[MAXNODE];

std::vector<long long> vals;

int insert(int prev, int low, int high, int pos, long long value)
{
    int cur = ++tot;
    lc[cur] = lc[prev];
    rc[cur] = rc[prev];
    sm[cur] = sm[prev] + value;
    if (low == high)
        return cur;
    int mid = (low + high) >> 1;
    if (pos <= mid)
        lc[cur] = insert(lc[prev], low, mid, pos, value);
    else
        rc[cur] = insert(rc[prev], mid + 1, high, pos, value);
    return cur;
}

void go(int u, int v, int low, int high)
{
    long long s = sm[u] - sm[v];
    if (s == 0)
        return;
    if (vals[low] > reach + 1)
        return;
    if (vals[high] <= reach + 1)
    {
        reach += s;
        return;
    }
    int mid = (low + high) >> 1;
    go(lc[u], lc[v], low, mid);
    go(rc[u], rc[v], mid + 1, high);
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;

    std::vector<long long> x(n + 1);
    for (int i = 1; i <= n; ++i)
        std::cin >> x[i];

    vals.assign(x.begin() + 1, x.end());
    std::sort(vals.begin(), vals.end());
    vals.erase(std::unique(vals.begin(), vals.end()), vals.end());
    int m = static_cast<int>(vals.size());

    int root[n + 1];
    root[0] = 0;
    for (int i = 1; i <= n; ++i)
    {
        int pos = static_cast<int>(std::lower_bound(vals.begin(), vals.end(), x[i]) - vals.begin());
        root[i] = insert(root[i - 1], 0, m - 1, pos, x[i]);
    }

    int a, b;
    while (q--)
    {
        std::cin >> a >> b;
        reach = 0;
        go(root[b], root[a - 1], 0, m - 1);
        std::cout << reach + 1 << '\n';
    }

    return 0;
}