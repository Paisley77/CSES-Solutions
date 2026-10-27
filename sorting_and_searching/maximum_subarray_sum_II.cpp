#include <iostream>
#include <queue>
#include <limits>

const long long NEG_INF = std::numeric_limits<long long>::min() / 4;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, a, b;
    std::cin >> n >> a >> b;
    std::vector<int> vec(n);
    for (auto &x : vec)
        std::cin >> x;
    std::vector<long long> prefix(n + 1, 0);
    for (int i = 1; i <= n; ++i)
        prefix[i] = prefix[i - 1] + vec[i - 1];
    std::deque<int> dq;
    long long max_sum = NEG_INF;
    for (int r = a; r <= n; ++r)
    {
        int add = r - a;
        /*
        Pop from the index with the largest prefix;
        back valid => add valid
        If prefix[back] >= prefix[add], then back can never be a better index
        */
        while (!dq.empty() && prefix[dq.back()] >= prefix[add])
            dq.pop_back();
        dq.push_back(add); // prefix[add] is the largest in queue
        if (dq.front() < r - b)
            dq.pop_front();
        max_sum = std::max(max_sum, prefix[r] - prefix[dq.front()]);
    }
    std::cout << max_sum << '\n';
    return 0;
}