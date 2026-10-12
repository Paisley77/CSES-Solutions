#include <vector>
#include <algorithm>
#include <string>
#include <iostream>

const int pattern6x5[6][5] = {
    {0, 0, 2, 3, 3},
    {1, 0, 2, 2, 3},
    {1, 1, 5, 5, 4},
    {6, 6, 5, 4, 4},
    {7, 6, 8, 9, 9},
    {7, 7, 8, 8, 9}};

const int pattern9x5[9][5] = {
    {0, 0, 2, 3, 3},
    {1, 0, 2, 2, 3},
    {1, 1, 5, 5, 4},
    {6, 6, 5, 4, 4},
    {6, 14, 14, 7, 7},
    {13, 13, 14, 10, 7},
    {13, 12, 12, 10, 10},
    {8, 12, 11, 11, 9},
    {8, 8, 11, 9, 9}};

class Builder
{
public:
    int tiles;
    std::vector<std::vector<int>> id;

    explicit Builder(int h, int w)
    {
        tiles = 0;
        id.assign(h, std::vector<int>(w, -1));
    }

    void put3x2(int r, int c)
    {
        int a = tiles++;
        int b = tiles++;
        id[r][c] = a;
        id[r][c + 1] = a;
        id[r + 1][c] = a;
        id[r + 1][c + 1] = b;
        id[r + 2][c] = b;
        id[r + 2][c + 1] = b;
    }

    void put2x3(int r, int c)
    {
        int a = tiles++;
        int b = tiles++;
        id[r][c] = a;
        id[r + 1][c] = a;
        id[r][c + 1] = a;
        id[r + 1][c + 1] = b;
        id[r][c + 2] = b;
        id[r + 1][c + 2] = b;
    }

    void putHx2(int r, int c, int bh)
    {
        for (int i = 0; i < bh; i += 3)
            put3x2(r + i, c);
    }

    void putPattern5(int r, int c, int bh)
    {
        const int (*pattern)[5];
        int cnt;
        if (bh == 6)
        {
            pattern = pattern6x5;
            cnt = 10;
        }
        else
        {
            pattern = pattern9x5;
            cnt = 15;
        }
        for (int i = 0; i < bh; ++i)
            for (int j = 0; j < 5; ++j)
                id[r + i][c + j] = tiles + pattern[i][j];
        tiles += cnt;
    }
};

std::vector<std::string> construct(int h, int w)
{
    Builder b(h, w);
    if (h == 3)
    { // w must be even
        for (int c = 0; c < w; c += 2)
            b.put3x2(0, c);
    }
    else if (w == 3)
    {
        for (int r = 0; r < h; r += 2)
            b.put2x3(r, 0);
    }
    else
    {
        // h = 6 k1 + 9 k2, where k2 = 0 or 1
        // w = 2 l1 + 5 l2, where l2 = 0 or 1
        std::vector<int> heights, widths;
        int remainingH = h, remainingW = w;
        if (h % 6 == 3)
        {
            heights.push_back(9);
            remainingH -= 9;
        }
        if (w % 2 == 1)
        {
            widths.push_back(5);
            remainingW -= 5;
        }
        while (remainingH > 0)
        {
            heights.push_back(6);
            remainingH -= 6;
        }
        while (remainingW > 0)
        {
            widths.push_back(2);
            remainingW -= 2;
        }
        int r = 0;
        for (int bh : heights)
        {
            int c = 0;
            for (int bw : widths)
            {
                if (bw == 2)
                    b.putHx2(r, c, bh);
                else
                    b.putPattern5(r, c, bh);
                c += bw;
            }
            r += bh;
        }
    }
    // build graph where each vertex is a tromino;
    // add an edge if two trominos are adjacent
    std::vector<std::vector<int>> adj(b.tiles);

    auto add_edge = [&](int u, int v)
    {
        if (u == v)
            return;
        adj[u].push_back(v);
        adj[v].push_back(u);
    };

    for (int r = 0; r < h; ++r)
    {
        for (int c = 0; c < w; ++c)
        {
            if (r + 1 < h)
                add_edge(b.id[r][c], b.id[r + 1][c]);
            if (c + 1 < w)
                add_edge(b.id[r][c], b.id[r][c + 1]);
        }
    }

    // remove duplicate neighbors
    for (auto &v : adj)
    {
        std::sort(v.begin(), v.end());
        v.erase(std::unique(v.begin(), v.end()), v.end());
    }

    // at most 9 colors are needed to ensure adjacent trominos have different colors
    std::vector<int> color(b.tiles, -1);
    for (int i = 0; i < b.tiles; ++i)
    {

        std::vector<char> used(9, 0);
        for (int j : adj[i])
            if (color[j] != -1)
                used[color[j]] = 1;
        for (int k = 0; k < 9; ++k)
            if (!used[k])
                color[i] = k;
    }

    std::vector<std::string> grid(h, std::string(w, '0'));
    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            grid[i][j] = char('A' + color[b.id[i][j]]);
        }
    }

    return grid;
}

bool is_possible(int n, int m)
{
    int l = std::min(n, m);
    int r = std::max(n, m);
    if ((n * m) % 3 != 0)
        return false;
    if (l == 1)
        return false;
    if (l == 3 && (r % 2 != 0))
        return false;
    return true;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int t;
    std::cin >> t;
    while (t--)
    {

        int n, m;
        std::cin >> n >> m;
        if (!is_possible(n, m))
        {
            std::cout << "NO\n";
            continue;
        }
        std::cout << "YES\n";
        int h = n, w = m;
        bool transposed = false;
        if (n % 3 != 0)
        {
            h = m;
            w = n;
            transposed = true;
        }

        std::vector<std::string> grid = construct(h, w);
        if (transposed)
        {
            std::vector<std::string> original(n, std::string(m, '0'));
            for (int i = 0; i < n; ++i)
            {
                for (int j = 0; j < m; ++j)
                    original[i][j] = grid[j][i];
            }
            grid.swap(original);
        }

        for (int i = 0; i < n; ++i)
            std::cout << grid[i] << '\n';
    }
    return 0;
}