#include <iostream>
#include <vector>

const int BITS = 17;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, k;
    std::cin >> n >> k;
    std::vector<int> a(n);
    for (auto &x : a)
        std::cin >> x;

    int d = n - k, L = n;
    for (int i = BITS; i >= 0; --i)
    {
        if ((d >> i) & 1)
        {
            int step = 1 << i;
            int newL = L - step;
            for (int j = 0; j < newL; ++j)
            {
                a[j] ^= a[j + step];
            }
            L = newL;
        }
    }

    for (int i = 0; i < k; ++i)
        std::cout << a[i] << " \n"[i == k - 1];
    return 0;
}