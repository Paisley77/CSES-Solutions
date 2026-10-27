#include <iostream>
#include <vector>
#include <algorithm>

typedef long long int64;

class Fenwick
{
private:
    std::vector<int> bit;

public:
    Fenwick(int n) : bit(n + 1, 0) {}
    void add(int idx, int delta)
    {
        for (; idx < (int)bit.size(); idx += (idx & -idx))
            bit[idx] += delta;
    }
    int query(int idx) const
    {
        int sum = 0;
        for (; idx > 0; idx -= (idx & -idx))
            sum += bit[idx];
        return sum;
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, k;
    std::cin >> n >> k;
    std::vector<int> a(n);
    for (auto &x : a)
        std::cin >> x;
    std::vector<int> value = a;
    std::sort(value.begin(), value.end());
    value.erase(std::unique(value.begin(), value.end()), value.end());
    std::vector<int> rank(n);
    for (int i = 0; i < n; ++i)
        rank[i] = std::lower_bound(value.begin(), value.end(), a[i]) - value.begin() + 1;
    Fenwick fenwick((int)value.size());
    int64 inversions = 0;
    for (int i = 0; i < k; ++i)
    {
        inversions += (i - fenwick.query(rank[i]));
        fenwick.add(rank[i], 1);
    }
    std::cout << inversions << ' ';
    for (int i = k; i < n; ++i)
    {
        inversions -= fenwick.query(rank[i - k] - 1);
        fenwick.add(rank[i - k], -1);
        inversions += (k - 1 - fenwick.query(rank[i]));
        fenwick.add(rank[i], 1);
        std::cout << inversions << ' ';
    }
    return 0;
}