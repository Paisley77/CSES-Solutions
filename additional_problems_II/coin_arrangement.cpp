#include <iostream>
#include <vector>
#include <algorithm>

long long dist(long long v, long long L, long long R) { return (v < L) ? L - v : 0 + (v > R) ? v - R
                                                                                             : 0; }

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    std::vector<long long> top(n + 1), bottom(n + 1), T(n + 1, 0), B(n + 1, 0);
    for (int i = 1; i <= n; ++i)
        std::cin >> top[i];
    for (int i = 1; i <= n; ++i)
        std::cin >> bottom[i];
    for (int i = 1; i <= n; ++i)
    {
        T[i] = T[i - 1] + top[i] - 1;
        B[i] = B[i - 1] + bottom[i] - 1;
    }
    long long C, L, R;
    C = L = R = 0;
    for (int i = 1; i <= n; ++i)
    {
        std::vector<long long> vec = {L, R, T[i], T[i], -B[i], -B[i]};
        std::sort(vec.begin(), vec.end());
        long long L_ = vec[2];
        long long R_ = vec[3];
        C = C + dist(L_, L, R) + abs(T[i] - L_) + abs(B[i] + L_);
        L = L_;
        R = R_;
    }
    std::cout << C << '\n';
    return 0;
}