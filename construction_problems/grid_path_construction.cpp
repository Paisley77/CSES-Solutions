#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>

const int DY[4] = {0, 1, 0, -1};
const int DX[4] = {1, 0, -1, 0};
const char DC[5] = "RDLU";

int dirMap(char c)
{
    if (c == 'R')
        return 0;
    if (c == 'D')
        return 1;
    if (c == 'L')
        return 2;
    return 3;
}

bool possible(int n, int m, int sy, int sx, int ty, int tx)
{
    if (n == 1 && m == 1)
        return sy == ty && sx == tx;
    if (sy == ty && sx == tx)
        return false;
    /*
    Normalize --> n <= m
    Rotation + Vertical Reflection
    */
    if (n > m)
    {
        std::swap(n, m);
        std::swap(sx, sy);
        std::swap(tx, ty);
    }
    /*
    Normalize --> sx <= tx
    Swapping endpoints
    */
    if (sx > tx)
    {
        std::swap(sx, tx);
        std::swap(sy, ty);
    }
    if (n == 1)
        return sx == 0 && tx == m - 1;
    if (n == 2)
    {
        // s & t in the same column => must both be in the leftmost or rightmost column
        if (sx == tx)
            return sx == 0 || sx == m - 1;
        return ((sx + sy) & 1) != ((tx + ty) & 1);
    }
    if ((n * m) & 1)
        return (((sx + sy) & 1) == 0) && (((tx + ty) & 1) == 0);
    if (n == 3)
    {
        if (((sx + sy) & 1) == ((tx + ty) & 1))
            return false;
        if ((((sx + sy) & 1) == 1) && ((sx < tx - 1) || (sy == 1 && (sx < tx))))
            return false;
        return true;
    }
    return ((sx + sy) & 1) != ((tx + ty) & 1);
}

std::string build_small_path(int n, int m, int sy, int sx, int ty, int tx)
{
    int N = n * m;
    int S = sy * m + sx;
    int T = ty * m + tx;
    std::vector<std::vector<std::pair<int, char>>> trace(N);
    std::vector<unsigned int> adjMask(N, 0);
    for (int y = 0; y < n; ++y)
    {
        for (int x = 0; x < m; ++x)
        {
            int u = y * m + x;
            for (int d = 0; d < 4; ++d)
            {
                int ny = y + DY[d];
                int nx = x + DX[d];
                if (ny >= 0 && ny < n && nx >= 0 && nx < m)
                {
                    int v = nx + ny * m;
                    trace[u].push_back({v, DC[d]});
                    adjMask[u] |= (1u << v);
                }
            }
        }
    }
    unsigned int ALL = (1u << N) - 1;
    auto feasible = [&](int cur, unsigned int mask, int depth) -> bool
    {
        if (depth == N)
            return cur == T;
        if (cur == T)
            return false;
        unsigned int rem = (ALL ^ mask) | (1u << cur);
        unsigned int seen = (1u << cur);
        unsigned int q = seen;
        while (q)
        {
            int u = __builtin_ctz(q);
            q &= (q - 1);
            unsigned int add = adjMask[u] & rem & ~seen;
            seen |= add;
            q |= add;
        }
        if (seen != rem)
            return false; // unable to visit all remaining squares
        unsigned int temp = rem;
        while (temp)
        {
            int u = __builtin_ctz(temp);
            temp &= (temp - 1);
            int deg = __builtin_popcount(adjMask[u] & rem);
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
    std::string path;
    path.reserve(N - 1);
    std::function<bool(int, unsigned int, int)> dfs = [&](int u, unsigned int mask, int depth)
    {
        // pre: feasible(u, mask, depth) == true
        if (depth == N)
            return u == T;
        if (u == T)
            return false;
        std::vector<std::tuple<int, int, char>> cand;
        for (auto [v, c] : trace[u])
        {
            if (mask & (1u << v))
                continue;
            if (v == T && depth + 1 < N)
                continue;
            int score = __builtin_popcount(adjMask[v] & ~(mask | (1u << v)));
            cand.push_back({score, v, c});
        }
        std::sort(cand.begin(), cand.end());
        for (auto [s, v, c] : cand)
        {
            path.push_back(c);
            unsigned int nmask = mask | (1u << v);
            if (feasible(v, nmask, depth + 1) && dfs(v, nmask, depth + 1))
                return true; // feasible chain continues from v; path built up in recursive calls
            path.pop_back();
        }
        return false;
    };
    dfs(S, 1u << S, 1);
    return path;
}

std::string solve_path(int n, int m, int sy, int sx, int ty, int tx)
{
    // pre: path exists
    if (n > m)
    {
        // rotate 90 degrees counterclockwise
        int nsy = m - 1 - sx;
        int nsx = sy;
        int nty = m - 1 - tx;
        int ntx = ty;
        std::string res = solve_path(m, n, nsy, nsx, nty, ntx);
        for (char &c : res)
        {
            if (c == 'R')
                c = 'D';
            else if (c == 'D')
                c = 'L';
            else if (c == 'L')
                c = 'U';
            else
                c = 'R';
        }
        return res;
    }
    if (sx > tx)
    {
        // reflect horizontally
        std::string res = solve_path(n, m, sy, m - 1 - sx, ty, m - 1 - tx);
        for (char &c : res)
        {
            if (c == 'R')
                c = 'L';
            else if (c == 'L')
                c = 'R';
        }
        return res;
    }
    if (sy > ty)
    {
        std::string res = solve_path(n, m, n - 1 - sy, sx, n - 1 - ty, tx);
        for (char &c : res)
        {
            if (c == 'U')
                c = 'D';
            else if (c == 'D')
                c = 'U';
        }
        return res;
    }
    if (n == 1)
        return std::string(m - 1, 'R');
    if (n == 2 && sy == ty && sx + 1 == tx)
    {
        std::string res;
        if (sy == 0)
        {
            res.append(sx, 'L');
            res += "D";
            res.append(m - 1, 'R');
            res += "U";
            res.append(m - tx - 1, 'L');
        }
        else
        {
            res.append(sx, 'L');
            res += "U";
            res.append(m - 1, 'R');
            res += "D";
            res.append(m - tx - 1, 'L');
        }
        return res;
    }
    if (n <= 4 && m <= 5)
        return build_small_path(n, m, sy, sx, ty, tx);
    // split out left strip
    if (sx >= 2 && possible(n, m - 2, sy, sx - 2, ty, tx - 2))
    {
        std::string res = solve_path(n, m - 2, sy, sx - 2, ty, tx - 2);
        int cx = sx - 2, cy = sy;
        for (int i = 0; i < (int)res.size(); ++i)
        {
            int nx = cx + DX[dirMap(res[i])];
            int ny = cy + DY[dirMap(res[i])];
            if (cx == 0 && nx == 0)
            {
                std::string strip = solve_path(n, 2, cy, 1, ny, 1);
                return res.substr(0, i) + "L" + strip + "R" + res.substr(i + 1);
            }
            cx = nx;
            cy = ny;
        }
    }
    // split out right strip
    if (tx < m - 2 && possible(n, m - 2, sy, sx, ty, tx))
    {
        std::string res = solve_path(n, m - 2, sy, sx, ty, tx);
        int cx = sx, cy = sy;
        for (int i = 0; i < (int)res.size(); ++i)
        {
            int nx = cx + DX[dirMap(res[i])];
            int ny = cy + DY[dirMap(res[i])];
            if (cx == m - 3 && nx == m - 3)
            {
                std::string strip = solve_path(n, 2, cy, 0, ny, 0);
                return res.substr(0, i) + "R" + strip + "L" + res.substr(i + 1);
            }
            cx = nx;
            cy = ny;
        }
    }
    // s and t must be distinct points
    // split into upper & lower parts
    for (int cut = sy; cut < ty; ++cut)
    {
        for (int x = 0; x < m; ++x)
        {
            if (possible(cut + 1, m, sy, sx, cut, x) && possible(n - cut - 1, m, 0, x, ty - cut - 1, tx))
            {
                std::string upper = solve_path(cut + 1, m, sy, sx, cut, x);
                std::string lower = solve_path(n - cut - 1, m, 0, x, ty - cut - 1, tx);
                return upper + "D" + lower;
            }
        }
    }
    // split into left & right parts
    for (int cut = sx; cut < tx; ++cut)
    {
        for (int y = 0; y < n; ++y)
        {
            if (possible(n, cut + 1, sy, sx, y, cut) && possible(n, m - cut - 1, y, 0, ty, tx - cut - 1))
            {
                std::string left = solve_path(n, cut + 1, sy, sx, y, cut);
                std::string right = solve_path(n, m - cut - 1, y, 0, ty, tx - cut - 1);
                return left + "R" + right;
            }
        }
    }
    return ""; // should not happen
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int t;
    std::cin >> t;
    while (t--)
    {
        int n, m, sy, sx, ty, tx;
        std::cin >> n >> m >> sy >> sx >> ty >> tx;
        --sy;
        --sx;
        --ty;
        --tx;
        if (possible(n, m, sy, sx, ty, tx))
            std::cout << "YES\n"
                      << solve_path(n, m, sy, sx, ty, tx) << '\n';
        else
            std::cout << "NO\n";
    }
    return 0;
}