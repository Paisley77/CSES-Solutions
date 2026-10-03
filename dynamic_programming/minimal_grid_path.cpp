#include <iostream>
#include <vector>
#include <string>
int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    std::vector<std::string> grid(n);
    for (auto &s : grid)
        std::cin >> s;
    std::vector<int> cur, next;
    std::string ans;
    ans.append(1, grid[0][0]);
    cur.push_back(0);
    for (int d = 0; d < 2 * n - 2; ++d)
    {
        // smallest letter at current anti-diagonal has been added to ans
        // appends smallest letter in the next anti-diagonal
        char smallest = 'Z' + 1;
        for (int r : cur)
        {
            int c = d - r;
            if (c < n - 1)
                smallest = std::min(smallest, grid[r][c + 1]);
            if (r < n - 1)
                smallest = std::min(smallest, grid[r + 1][c]);
        }
        ans.append(1, smallest);
        if (d == 2 * n - 3)
            break;
        next.clear();
        int last = -1;
        for (int r : cur)
        {
            int c = d - r;
            if (c < n - 1 && grid[r][c + 1] == smallest && r != last)
            {
                next.push_back(r);
                last = r;
            }
            if (r < n - 1 && grid[r + 1][c] == smallest)
            {
                next.push_back(r + 1);
                last = r + 1;
            }
        }
        cur.swap(next);
    }

    std::cout << ans;
    return 0;
}