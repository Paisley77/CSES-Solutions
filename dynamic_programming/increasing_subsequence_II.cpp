#include <iostream>
#include <vector>
#include <algorithm>

typedef long long int64;

const int MOD = 1e9 + 7;

class Fenwick
{
private:
    std::vector<int64> bit;

public:
    explicit Fenwick(int n) : bit(n + 1, 0) {}
    void add(int i, int64 val)
    {
        for (; i < (int)bit.size(); i += (i & -i))
            bit[i] = (bit[i] + val) % MOD;
    }
    int64 query(int i)
    {
        int64 total = 0;
        for (; i > 0; i -= (i & -i))
            total = (total + bit[i]) % MOD;
        return total;
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    std::vector<int64> arr(n);
    for (int64 &a : arr)
        std::cin >> a;
    std::vector<int64> values = arr;
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
    std::vector<int> rank(n);
    for (int i = 0; i < n; ++i)
        rank[i] = int(std::lower_bound(values.begin(), values.end(), arr[i]) - values.begin() + 1);
    Fenwick fenwick((int)values.size());
    int64 count = 0;
    for (int i = 0; i < n; ++i)
    {
        int64 stepCount = (fenwick.query(rank[i] - 1) + 1) % MOD;
        fenwick.add(rank[i], stepCount);
        count = (count + stepCount) % MOD;
    }
    std::cout << count << '\n';
    return 0;
}