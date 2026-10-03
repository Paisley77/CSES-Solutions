#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <set>
#include <cstdlib>

using ll = long long;
using line_type = std::set<std::pair<ll, ll>>;
using lines_type = std::map<ll, line_type>;
const ll INF = 2e11;

void mapInsert(std::map<ll, ll> &mp, ll a, ll b)
{
    auto it = mp.upper_bound(a);
    if (it != mp.begin() && std::prev(it)->second >= b)
        return;
    mp[a] = b;
    while (it != mp.end() && it->second <= b)
        it = mp.erase(it);
}

bool mapCovers(const std::map<ll, ll> &mp, ll x)
{
    auto it = mp.upper_bound(x);
    if (it == mp.begin())
        return false;
    return std::prev(it)->second >= x;
}

struct Stab
{
    int n;
    std::vector<std::map<ll, ll>> node;

    void init(int n_)
    {
        n = n_;
        node.assign(4 * n, {});
    }

    void insertNode(int idx, int lo, int hi, int pos, ll a, ll b)
    {
        mapInsert(node[idx], a, b);
        if (lo == hi)
            return;
        int mid = (lo + hi) >> 1;
        if (pos <= mid)
            insertNode(2 * idx, lo, mid, pos, a, b);
        else
            insertNode(2 * idx + 1, mid + 1, hi, pos, a, b);
    }

    void insert(int pos, ll a, ll b) { insertNode(1, 0, n - 1, pos, a, b); }

    int qleft(int idx, int lo, int hi, int left, int right, ll coord)
    {
        if (lo > right || hi < left || !mapCovers(node[idx], coord))
            return -1;
        if (lo == hi)
            return lo;
        int res = -1;
        int mid = (lo + hi) >> 1;
        if (left <= mid)
            res = qleft(2 * idx, lo, mid, left, right, coord);
        if (res != -1)
        {
            return res;
        }
        return qleft(2 * idx + 1, mid + 1, hi, left, right, coord);
    }

    int qright(int idx, int lo, int hi, int left, int right, ll coord)
    {
        if (lo > right || hi < left || !mapCovers(node[idx], coord))
            return -1;
        if (lo == hi)
            return lo;
        int mid = (lo + hi) >> 1;
        int res = -1;
        if (right > mid)
            res = qright(2 * idx + 1, mid + 1, hi, left, right, coord);
        if (res != -1)
            return res;
        return qright(2 * idx, lo, mid, left, right, coord);
    }

    int leftmost(int left, int right, ll coord) { return qleft(1, 0, n - 1, left, right, coord); }
    int rightmost(int left, int right, ll coord) { return qright(1, 0, n - 1, left, right, coord); }
};

void add(line_type &line, ll a, ll b)
{
    if (a > b)
        std::swap(a, b);
    auto it = line.lower_bound({a, a});
    if (it != line.begin() && std::prev(it)->second + 1 >= a)
    {
        it = std::prev(it);
    }
    ll L = a, R = b;
    while (it != line.end() && it->first <= (R + 1))
    {
        L = std::min(L, it->first);
        R = std::max(R, it->second);
        it = line.erase(it);
    }
    line.insert({L, R});
}

int id(const std::vector<ll> &s, ll v)
{
    return int(std::lower_bound(s.begin(), s.end(), v) - s.begin());
}

int check_perp(Stab &perpStab, int fromID, int toID, ll coord, bool &shouldStop)
{
    int dir = (fromID <= toID) ? 1 : -1;
    int stop;
    if (dir == 1)
        stop = perpStab.leftmost(fromID + 1, toID, coord);
    else
        stop = perpStab.rightmost(toID, fromID - 1, coord);
    if (stop != -1)
        shouldStop = true;
    return stop;
}

ll check_paral(line_type &line, ll from, ll to, bool &shouldStop)
{
    ll stop = to;
    int dir = (to >= from) ? 1 : -1;
    if (dir == 1)
    {
        auto it = line.upper_bound({from, INF});
        if (it != line.begin() && std::prev(it)->second > from)
            it = std::prev(it);
        if (it != line.end() && it->first <= to)
        {
            stop = it->first;
            shouldStop = true;
        }
    }
    else
    {
        auto it = line.lower_bound({from, -INF});
        if (it != line.begin() && (--it)->second >= to)
        {
            stop = it->second;
            shouldStop = true;
        }
    }
    return stop;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    std::vector<char> dir(n);
    std::vector<ll> len_(n);
    for (int i = 0; i < n; ++i)
    {
        std::cin >> dir[i] >> len_[i];
    }

    std::vector<ll> xs{0}, ys{0};
    {
        ll cur_x = 0, cur_y = 0;
        for (int i = 0; i < n; ++i)
        {
            if (dir[i] == 'R')
                cur_x += len_[i];
            else if (dir[i] == 'L')
                cur_x -= len_[i];
            else if (dir[i] == 'U')
                cur_y += len_[i];
            else
                cur_y -= len_[i];
            xs.push_back(cur_x);
            ys.push_back(cur_y);
        }
    }

    std::sort(xs.begin(), xs.end());
    std::sort(ys.begin(), ys.end());
    xs.erase(std::unique(xs.begin(), xs.end()), xs.end());
    ys.erase(std::unique(ys.begin(), ys.end()), ys.end());

    lines_type row, col;

    Stab rowStab, colStab;
    rowStab.init((int)ys.size());
    colStab.init((int)xs.size());

    ll ox = 0, oy = 0, x = 0, y = 0;
    bool shouldStop = false;
    ll ans = 0;

    for (int i = 0; i < n && !shouldStop; ++i)
    {
        char d = dir[i];
        ll l = len_[i];
        if (d == 'R' || d == 'L')
        {
            if (d == 'R')
                x = check_paral(row[oy], ox, ox + l, shouldStop);
            else
                x = check_paral(row[oy], ox, ox - l, shouldStop);
            int stopID = check_perp(colStab, id(xs, ox), id(xs, x), oy, shouldStop);
            if (stopID != -1)
                x = xs[stopID];
            if (x != ox)
            {
                add(row[oy], ox, x);
                rowStab.insert(id(ys, oy), std::min(ox, x), std::max(ox, x));
            }
        }
        else
        {
            if (d == 'U')
                y = check_paral(col[ox], oy, oy + l, shouldStop);
            else
                y = check_paral(col[ox], oy, oy - l, shouldStop);
            int stopID = check_perp(rowStab, id(ys, oy), id(ys, y), ox, shouldStop);
            if (stopID != -1)
                y = ys[stopID];
            if (y != oy)
            {
                add(col[ox], oy, y);
                colStab.insert(id(xs, ox), std::min(oy, y), std::max(oy, y));
            }
        }
        ans += std::llabs(x - ox) + std::llabs(y - oy);
        ox = x;
        oy = y;
    }

    std::cout << ans << '\n';
    return 0;
}