#include <iostream>
#include <vector>

const int BITS = 30;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<int> basis(BITS, 0);
    int cur, rank = 0;

    for (int i = 0; i < n; ++i)
    {
        std::cin >> cur;
        for (int b = BITS - 1; b >= 0; --b)
        {
            if (!((cur >> b) & 1))
                continue;
            if (basis[b] == 0)
            {
                basis[b] = cur;
                rank++;
                break;
            }
            cur ^= basis[b];
        }
    }

    int ans = 1;
    for (int i = 0; i < rank; ++i)
        ans = ans * 2;

    std::cout << ans << std::endl;

    return 0;
}