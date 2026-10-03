#include <iostream>
#include <vector>

int n;
std::vector<long long> bit;

void add(int i, long long d)
{
    for (; i <= n; i += (i & (-i)))
        bit[i] += d;
}

long long query(int i)
{
    long long sm = 0;
    for (; i > 0; i -= (i & (-i)))
        sm += bit[i];
    return sm;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int q;
    std::cin >> n >> q;

    std::vector<long long> val(n + 1);
    for (int i = 1; i <= n; ++i)
        std::cin >> val[i];

    std::vector<std::vector<int>> g(n + 1);
    for (int i = 1; i <= (n - 1); ++i)
    {
        int a, b;
        std::cin >> a >> b;
        g[a].push_back(b);
        g[b].push_back(a);
    }

    std::vector<int> par(n + 1, 0), tin(n + 1), sz(n + 1, 1), order;
    order.reserve(n);
    {
        std::vector<int> stk;
        stk.reserve(n);
        stk.push_back(1);
        std::vector<char> vst(n + 1, 0);
        vst[1] = 1;
        while (!stk.empty())
        {
            int v = stk.back();
            stk.pop_back();
            tin[v] = static_cast<int>(order.size()) + 1;
            order.push_back(v);
            for (int u : g[v])
            {
                if (!vst[u])
                {
                    vst[u] = 1;
                    par[u] = v;
                    stk.push_back(u);
                }
            }
        }
    }

    bit.assign(n + 1, 0);
    for (int i = 1; i <= n; ++i)
        bit[tin[i]] = val[i];
    for (int i = 1; i <= n; ++i)
    {
        int j = i + (i & (-i));
        if (j <= n)
            bit[j] += bit[i];
    }

    for (int i = n - 1; i > 0; --i)
    {
        int v = order[i];
        sz[par[v]] += sz[v];
    }

    while (q--)
    {
        int type;
        std::cin >> type;
        if (type == 1)
        {
            int s;
            long long x;
            std::cin >> s >> x;
            add(tin[s], x - val[s]);
            val[s] = x;
        }
        else
        {
            int s;
            std::cin >> s;
            int l = tin[s], r = tin[s] + sz[s] - 1;
            long long sm = query(r) - query(l - 1);
            std::cout << sm << '\n';
        }
    }

    return 0;
}