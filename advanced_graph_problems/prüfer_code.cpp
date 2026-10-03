#include <iostream>
#include <vector>
#include <set>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    std::vector<int> code(n - 2), degree(n + 1, 1);
    for (int i = 0; i < n - 2; ++i)
    {
        std::cin >> code[i];
        degree[code[i]]++;
    }
    std::set<int> leaf_set;
    for (int i = 1; i <= n; ++i)
        if (degree[i] == 1)
            leaf_set.insert(i);
    for (int i = 0; i < (n - 2); ++i)
    {
        int leaf = *leaf_set.begin();
        std::cout << code[i] << " " << leaf << '\n';
        leaf_set.erase(leaf_set.begin());
        if (--degree[code[i]] == 1)
            leaf_set.insert(code[i]);
    }

    std::cout << *leaf_set.begin() << " " << *leaf_set.rbegin();

    return 0;
}