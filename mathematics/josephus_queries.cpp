#include <iostream>

int JosephusSolver(int n, int k)
{
    int firstPhase = (n + 1) / 2;
    int child = 2 * k;
    if (k <= firstPhase)
        return (child <= n) ? child : 1;
    int prev = JosephusSolver(n / 2, k - firstPhase);
    return (n % 2 == 0) ? 2 * prev - 1 : 2 * prev + 1;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int q;
    std::cin >> q;
    while (q--)
    {
        int n, k;
        std::cin >> n >> k;
        std::cout << JosephusSolver(n, k) << '\n';
    }
    return 0;
}