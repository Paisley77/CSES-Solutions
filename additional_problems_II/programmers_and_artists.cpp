#include <iostream>
#include <queue>
#include <algorithm>

typedef long long int64;

const int64 NEG = -((1LL << 60) - 1);

struct Employee
{
    int64 x;
    int64 y;
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int a, b, n;
    std::cin >> a >> b >> n;
    std::vector<Employee> grapes(n);
    for (auto &[reason, emotion] : grapes)
        std::cin >> reason >> emotion;
    // sort by advantage of being a programmer over an artist
    std::sort(grapes.begin(),
              grapes.end(),
              [](const Employee &apple, const Employee &orange)
              { return apple.x - apple.y > orange.x - orange.y; });
    /*
    pref[s] = max sum of a 'x values' from 0 ... s-1 for s = a,...,n
    pref[s] = NEG for s = 0, ..., a-1
    special case: pref[s] = 0 for all s if a = 0
    */
    std::vector<int64> pref(n + 1, NEG);
    if (a == 0)
        std::fill(pref.begin(), pref.end(), 0);
    else
    {
        std::priority_queue<int64, std::vector<int64>, std::greater<int64>> pq;
        int64 sum = 0;
        for (int s = 0; s < n; ++s)
        {
            sum += grapes[s].x;
            pq.push(grapes[s].x);
            if ((int)pq.size() > a)
            {
                sum -= pq.top();
                pq.pop();
            }
            if ((int)pq.size() == a)
                pref[s + 1] = sum;
        }
    }
    /*
    suff[s] = max sum of b 'y values' from s ... n-1 for s = 0,...,n-b
    suff[s] = NEG for s = n-b+1,...,n
    special case: suff[s] = 0 for all s if b = 0
    */
    std::vector<int64> suff(n + 1, NEG);
    if (b == 0)
        std::fill(suff.begin(), suff.end(), 0);
    else
    {
        std::priority_queue<int64, std::vector<int64>, std::greater<int64>> pq;
        int64 sum = 0;
        for (int s = n - 1; s >= 0; --s)
        {
            sum += grapes[s].y;
            pq.push(grapes[s].y);
            if ((int)pq.size() > b)
            {
                sum -= pq.top();
                pq.pop();
            }
            if ((int)pq.size() == b)
                suff[s] = sum;
        }
    }
    int64 ans = 0;
    /*
    For any boundary index s, divide into [0, ..., s - 1] U [s, ..., n]
    pref[s] records the maximum sum of a 'x values' from the first set
    suff[s] records the maximum sum of b 'y values' from the second set
    pref[s] + suff[s] = maximum skill sum if we set the boundary at s
    We simply need to find the optimal boundary s.
    */
    for (int s = a; s <= n - b; ++s)
        ans = std::max(pref[s] + suff[s], ans);
    std::cout << ans << '\n';
    return 0;
}