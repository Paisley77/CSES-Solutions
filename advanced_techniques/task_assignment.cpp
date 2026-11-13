#include <iostream>
#include <vector>

const int INF = 1000;

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    std::vector<std::vector<int>> cost(n + 1, std::vector<int>(n + 1));
    for (int i = 1; i <= n; ++i)
    {
        for (int j = 1; j <= n; ++j)
        {
            std::cin >> cost[i][j];
        }
    }
    std::vector<int> u(n + 1, 0), v(n + 1, 0);
    std::vector<int> p(n + 1, 0);
    std::vector<int> way(n + 1, 0);
    for (int i = 1; i <= n; ++i)
    {
        int j0 = 0;
        p[j0] = i;
        std::vector<int> minv(n + 1, INF);
        std::vector<char> used(n + 1, 0);
        // Expanding alternating search tree
        while (p[j0] != 0)
        {
            int i0 = p[j0];
            used[j0] = 1;
            int delta = INF;
            int j1 = 0;
            for (int j = 1; j <= n; ++j)
            {
                if (used[j])
                    continue;
                int r_i0_j = cost[i0][j] - u[i0] - v[j];
                if (r_i0_j < minv[j])
                {
                    minv[j] = r_i0_j;
                    way[j] = j0; // way[j] = task matched to argmin_{i \in S} r_{ij}
                }
                if (minv[j] < delta)
                {
                    delta = minv[j];
                    j1 = j; // new task to be visited
                }
            }
            for (int j = 0; j <= n; ++j)
            { // we include j = 0 because p[j] = s, our starting employee, which could potentially lead to a new task throug a tight edge
                if (used[j])
                {
                    u[p[j]] += delta;
                    v[j] -= delta;
                }
                else
                {
                    minv[j] -= delta;
                }
            }
            // at this point, minv[j1] = 0;
            j0 = j1;
        }
        // Flip matching status along augmenting path to increase number of edges by one
        while (j0 != 0)
        {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        }
    }
    int totalCost = 0;
    for (int i = 1; i <= n; ++i)
    {
        totalCost += (u[i] + v[i]);
    }
    std::cout << totalCost << '\n';
    for (int j = 1; j <= n; ++j)
    {
        std::cout << p[j] << " " << j << '\n';
    }
    return 0;
}