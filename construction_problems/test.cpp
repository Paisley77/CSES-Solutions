#include <bits/stdc++.h>
using namespace std;

const int DY[4] = {1, 0, -1, 0};
const int DX[4] = {0, 1, 0, -1};
const char DC[5] = "DRUL";

int dir_id(char c)
{
    if (c == 'D')
        return 0;
    if (c == 'R')
        return 1;
    if (c == 'U')
        return 2;
    return 3;
}

/*
    Exact existence test for a rectangular grid.

    Coordinates are 0-based.
*/
bool possible(int n, int m,
              int sy, int sx,
              int ty, int tx)
{
    // A 1x1 rectangle may occur inside the recursion.
    if (n == 1 && m == 1)
        return true;

    if (sy == ty && sx == tx)
        return false;

    /*
        Make n <= m.
    */
    if (n > m)
    {
        swap(n, m);
        swap(sy, sx);
        swap(ty, tx);
    }

    /*
        Put the left endpoint first.
    */
    if (sx > tx)
    {
        swap(sx, tx);
        swap(sy, ty);
    }

    /*
        1 x m:

        The path is forced, so the endpoints must
        be the two ends.
    */
    if (n == 1)
        return sx == 0 && tx == m - 1;

    /*
        2 x m special case.
    */
    if (n == 2)
    {
        // Same column is possible only at the boundary.
        if (sx == tx && (sx == 0 || sx == m - 1))
            return true;

        if (sx == tx)
            return false;

        // Area is even, so endpoint colors must differ.
        return ((sx + sy) & 1) != ((tx + ty) & 1);
    }

    /*
        Odd area => both dimensions are odd.

        (0,0) has the majority color, namely even parity.
    */
    if ((n * m) & 1)
    {
        return ((sx + sy) & 1) == 0 && ((tx + ty) & 1) == 0;
    }

    /*
        Special obstruction for 3 x even rectangles.
    */
    if (n == 3)
    {
        // Even area => endpoint colors must differ.
        if (((sx + sy) & 1) == ((tx + ty) & 1))
            return false;

        /*
            F3 obstruction.

            Remember sx <= tx.
        */
        if (((sx + sy) & 1) == 1 &&
            (sx < tx - 1 || (sy == 1 && sx < tx)))
        {
            return false;
        }

        // Symmetric form, useful before/after transformations.
        if (((tx + ty) & 1) == 1 &&
            (tx < sx - 1 || (ty == 1 && tx < sx)))
        {
            return false;
        }

        return true;
    }

    /*
        For n >= 4, color compatibility is sufficient.
    */
    return ((sx + sy) & 1) != ((tx + ty) & 1);
}

/*
    Solve a tiny rectangle, at most 4 x 5.

    We use DFS with two strong necessary-condition prunings:
      1. all remaining cells must be connected;
      2. every future internal path vertex must still
         have degree >= 2.
*/
string brute_small(int n, int m,
                   int sy, int sx,
                   int ty, int tx)
{
    int N = n * m;

    int S = sy * m + sx;
    int T = ty * m + tx;

    vector<vector<pair<int, char>>> adj(N);
    vector<unsigned int> adjMask(N, 0);

    for (int y = 0; y < n; ++y)
    {
        for (int x = 0; x < m; ++x)
        {
            int u = y * m + x;

            for (int d = 0; d < 4; ++d)
            {
                int ny = y + DY[d];
                int nx = x + DX[d];

                if (0 <= ny && ny < n &&
                    0 <= nx && nx < m)
                {
                    int v = ny * m + nx;

                    adj[u].push_back({v, DC[d]});
                    adjMask[u] |= (1u << v);
                }
            }
        }
    }

    unsigned int ALL = (1u << N) - 1;

    string path;
    path.reserve(N - 1);

    auto feasible =
        [&](int cur,
            unsigned int mask,
            int depth) -> bool
    {
        if (depth == N)
            return cur == T;

        if (cur == T)
            return false;

        /*
            Remaining graph consists of:
              current cell + all unvisited cells.
        */
        unsigned int rem =
            (ALL ^ mask) | (1u << cur);

        /*
            Connectivity check.
        */
        unsigned int seen = 1u << cur;
        unsigned int q = seen;

        while (q)
        {
            int u = __builtin_ctz(q);
            q &= q - 1;

            unsigned int add =
                adjMask[u] & rem & ~seen;

            seen |= add;
            q |= add;
        }

        if (seen != rem)
            return false;

        /*
            The remaining path goes from cur to T.

            cur and T may have degree 1.
            Every other remaining vertex needs degree >= 2.
        */
        unsigned int tmp = rem;

        while (tmp)
        {
            int u = __builtin_ctz(tmp);
            tmp &= tmp - 1;

            int deg =
                __builtin_popcount(adjMask[u] & rem);

            if (u == cur || u == T)
            {
                if (deg < 1)
                    return false;
            }
            else
            {
                if (deg < 2)
                    return false;
            }
        }

        return true;
    };

    function<bool(int, unsigned int, int)> dfs =
        [&](int u,
            unsigned int mask,
            int depth) -> bool
    {
        if (depth == N)
            return u == T;

        if (u == T)
            return false;

        /*
            Try more constrained neighbors first.
        */
        vector<tuple<int, int, char>> cand;

        for (auto [v, c] : adj[u])
        {
            if (mask & (1u << v))
                continue;

            // We may enter T only on the last move.
            if (v == T && depth + 1 < N)
                continue;

            unsigned int future =
                adjMask[v] & ~(mask | (1u << v));

            int score =
                __builtin_popcount(future);

            cand.push_back({score, v, c});
        }

        sort(cand.begin(), cand.end());

        for (auto [score, v, c] : cand)
        {
            unsigned int nmask =
                mask | (1u << v);

            path.push_back(c);

            if ((depth + 1 == N ||
                 feasible(v, nmask, depth + 1)) &&
                dfs(v, nmask, depth + 1))
            {
                return true;
            }

            path.pop_back();
        }

        return false;
    };

    dfs(S, 1u << S, 1);

    return path;
}

string solve_path(int n, int m,
                  int sy, int sx,
                  int ty, int tx)
{
    /*
        Rotate 90 degrees so n <= m.

        old (y,x) -> new (m-1-x, y)
    */
    if (n > m)
    {
        int nsy = m - sx - 1;
        int nsx = sy;

        int nty = m - tx - 1;
        int ntx = ty;

        string res =
            solve_path(m, n,
                       nsy, nsx,
                       nty, ntx);

        /*
            Convert directions back:

            new D -> old L
            new R -> old D
            new U -> old R
            new L -> old U
        */
        for (char &c : res)
        {
            if (c == 'D')
                c = 'L';
            else if (c == 'R')
                c = 'D';
            else if (c == 'U')
                c = 'R';
            else
                c = 'U';
        }

        return res;
    }

    /*
        Horizontal reflection:
        make sx <= tx.
    */
    if (sx > tx)
    {
        string res =
            solve_path(
                n, m,
                sy, m - sx - 1,
                ty, m - tx - 1);

        for (char &c : res)
        {
            if (c == 'L')
                c = 'R';
            else if (c == 'R')
                c = 'L';
        }

        return res;
    }

    /*
        Vertical reflection:
        make sy <= ty.
    */
    if (sy > ty)
    {
        string res =
            solve_path(
                n, m,
                n - sy - 1, sx,
                n - ty - 1, tx);

        for (char &c : res)
        {
            if (c == 'U')
                c = 'D';
            else if (c == 'D')
                c = 'U';
        }

        return res;
    }

    /*
        1 x m.
    */
    if (n == 1)
        return string(m - 1, 'R');

    /*
        Useful explicit 2-row case:
        adjacent endpoints in the same row.
    */
    if (n == 2 &&
        sy == ty &&
        sx + 1 == tx)
    {
        string res;

        if (sy == 0)
        {
            /*
                Start on the top row.
            */
            res += string(sx, 'L');
            res += 'D';

            res += string(m - 1, 'R');

            res += 'U';
            res += string(m - 1 - tx, 'L');
        }
        else
        {
            /*
                Start on the bottom row.
            */
            res += string(sx, 'L');
            res += 'U';

            res += string(m - 1, 'R');

            res += 'D';
            res += string(m - 1 - tx, 'L');
        }

        return res;
    }

    /*
        Tiny irreducible rectangles.
    */
    if (n <= 4 && m <= 5)
    {
        return brute_small(
            n, m,
            sy, sx,
            ty, tx);
    }

    /*
        ------------------------------------------------
        Case 1: remove a 2-column strip from the left.
        ------------------------------------------------
    */
    if (sx >= 2 &&
        possible(
            n, m - 2,
            sy, sx - 2,
            ty, tx - 2))
    {
        string res =
            solve_path(
                n, m - 2,
                sy, sx - 2,
                ty, tx - 2);

        int cy = sy;
        int cx = sx - 2;

        /*
            Find a vertical edge on the left boundary
            of the recursively solved rectangle.
        */
        for (int i = 0;
             i < (int)res.size();
             ++i)
        {
            int d = dir_id(res[i]);

            int ny = cy + DY[d];
            int nx = cx + DX[d];

            if (cx == 0 && nx == 0)
            {
                /*
                    Replace

                        cur
                         |
                        next

                    by

                        cur -- left strip -- next.
                */
                string strip =
                    solve_path(
                        n, 2,
                        cy, 1,
                        ny, 1);

                return res.substr(0, i) + "L" + strip + "R" + res.substr(i + 1);
            }

            cy = ny;
            cx = nx;
        }
    }

    /*
        ------------------------------------------------
        Case 2: remove a 2-column strip from the right.
        ------------------------------------------------
    */
    if (tx <= m - 3 &&
        possible(
            n, m - 2,
            sy, sx,
            ty, tx))
    {
        string res =
            solve_path(
                n, m - 2,
                sy, sx,
                ty, tx);

        int cy = sy;
        int cx = sx;

        // Rightmost column in the reduced rectangle.
        int boundary = m - 3;

        for (int i = 0;
             i < (int)res.size();
             ++i)
        {
            int d = dir_id(res[i]);

            int ny = cy + DY[d];
            int nx = cx + DX[d];

            if (cx == boundary &&
                nx == boundary)
            {
                string strip =
                    solve_path(
                        n, 2,
                        cy, 0,
                        ny, 0);

                return res.substr(0, i) + "R" + strip + "L" + res.substr(i + 1);
            }

            cy = ny;
            cx = nx;
        }
    }

    /*
        ------------------------------------------------
        Case 3: split horizontally.

          upper rectangle
        ------------------- cut
          lower rectangle
    */
    for (int cut = sy;
         cut < ty;
         ++cut)
    {
        for (int x = 0; x < m; ++x)
        {
            int h1 = cut + 1;
            int h2 = n - h1;

            if (possible(
                    h1, m,
                    sy, sx,
                    cut, x) &&
                possible(
                    h2, m,
                    0, x,
                    ty - h1, tx))
            {
                string a =
                    solve_path(
                        h1, m,
                        sy, sx,
                        cut, x);

                string b =
                    solve_path(
                        h2, m,
                        0, x,
                        ty - h1, tx);

                return a + "D" + b;
            }
        }
    }

    /*
        ------------------------------------------------
        Case 4: split vertically.
        ------------------------------------------------
    */
    for (int cut = sx;
         cut < tx;
         ++cut)
    {
        for (int y = 0; y < n; ++y)
        {
            int w1 = cut + 1;
            int w2 = m - w1;

            if (possible(
                    n, w1,
                    sy, sx,
                    y, cut) &&
                possible(
                    n, w2,
                    y, 0,
                    ty, tx - w1))
            {
                string a =
                    solve_path(
                        n, w1,
                        sy, sx,
                        y, cut);

                string b =
                    solve_path(
                        n, w2,
                        y, 0,
                        ty, tx - w1);

                return a + "R" + b;
            }
        }
    }

    /*
        For a feasible rectangular-grid instance,
        the construction above always finds a case.
    */
    // assert(false);
    return "";
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--)
    {
        int n, m;
        int y1, x1, y2, x2;

        cin >> n >> m >> y1 >> x1 >> y2 >> x2;

        --y1;
        --x1;
        --y2;
        --x2;

        if (!possible(
                n, m,
                y1, x1,
                y2, x2))
        {
            cout << "NO\n";
            continue;
        }

        cout << "YES\n";

        cout << solve_path(
                    n, m,
                    y1, x1,
                    y2, x2)
             << '\n';
    }
}