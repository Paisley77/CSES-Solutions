#include <iostream>
#include <vector>

struct State
{
    int u;
    int p;
    int depth;
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, k1, k2;
    std::cin >> n >> k1 >> k2;
    std::vector<std::vector<int>> adj(n);
    for (int i = 0; i < n - 1; ++i)
    {
        int a, b;
        std::cin >> a >> b;
        a--;
        b--;
        adj[a].push_back(b);
        adj[b].push_back(a);
    }

    std::vector<int> component, parent(n), sub(n), all_depths, cur_depths, tasks;
    component.reserve(n);
    all_depths.reserve(n);
    cur_depths.reserve(n);
    tasks.reserve(n);
    std::vector<char> removed(n, 0);
    std::vector<State> depth_stack;
    depth_stack.reserve(n);
    std::vector<int> freq(n, 0);
    std::vector<long long> pref(n, 0);

    auto build_component = [&](int start)
    {
        component.clear();
        component.push_back(start);
        parent[start] = -1;
        for (int i = 0; i < (int)component.size(); ++i)
        {
            int u = component[i];
            for (int v : adj[u])
            {
                if (v != parent[u] && !removed[v])
                {
                    parent[v] = u;
                    component.push_back(v);
                }
            }
        }
    };

    auto calc_size = [&]()
    {
        for (int i = (int)component.size() - 1; i >= 0; --i)
        {
            int u = component[i];
            sub[u] = 1;
            for (int v : adj[u])
            {
                if (v != parent[u] && !removed[v])
                    sub[u] += sub[v];
            }
        }
    };

    auto find_centroid = [&]()
    {
        int total = (int)component.size();
        for (int u : component)
        {
            int largest = total - sub[u];
            for (int v : adj[u])
            {
                if (v != parent[u] && !removed[v])
                    largest = std::max(largest, sub[v]);
            }
            if (2 * largest <= total)
                return u;
        }
        return component[0];
    };

    auto compute_component_and_centroid = [&](int start)
    {
        build_component(start);
        calc_size();
        return find_centroid();
    };

    auto collect_depths = [&](int start, int parent, std::vector<int> &depths)
    {
        depths.clear();
        depth_stack.push_back({start, parent, 1});
        while (!depth_stack.empty())
        {
            State cur = depth_stack.back();
            depth_stack.pop_back();
            depths.push_back(cur.depth);
            if (cur.depth == k2)
                continue;
            for (int v : adj[cur.u])
            {
                if (v != cur.p && !removed[v])
                    depth_stack.push_back({v, cur.u, cur.depth + 1});
            }
        }
    };

    auto count_leq_prepared = [&](int K, int mx, const std::vector<int> &depths)
    {
        long long ans = 0;
        for (int d = 0; d <= std::min(mx, K); ++d)
        {
            int lim = std::min(mx, K - d);
            ans += freq[d] * pref[lim] * 1LL;
        }
        for (int d = 0; 2 * d <= K && d <= mx; ++d)
        {
            ans -= freq[d]; // self pairs
        }
        ans /= 2;
        return ans;
    };

    auto count_range = [&](const std::vector<int> &depths)
    {
        if (depths.size() < 2)
            return 0LL;
        int mx = 0;
        for (int d : depths)
        {
            freq[d]++;
            if (d > mx)
                mx = d;
        }
        int prefix = 0;
        for (int d = 0; d <= mx; ++d)
        {
            pref[d] = prefix + freq[d];
            prefix = pref[d];
        }
        long long res = count_leq_prepared(k2, mx, depths) - count_leq_prepared(k1 - 1, mx, depths);
        for (int d : depths)
            freq[d]--;
        return res;
    };

    auto solve_centroid = [&](int centroid)
    {
        all_depths.clear();
        all_depths.push_back(0);
        long long ans = 0;
        for (int v : adj[centroid])
        {
            if (removed[v])
                continue;
            collect_depths(v, centroid, cur_depths);
            ans -= count_range(cur_depths);
            all_depths.insert(all_depths.end(), cur_depths.begin(), cur_depths.end());
        }
        ans += count_range(all_depths);
        return ans;
    };

    auto decompose = [&]()
    {
        tasks.push_back(0);
        long long ans = 0;
        while (!tasks.empty())
        {
            int start = tasks.back();
            tasks.pop_back();
            int centroid = compute_component_and_centroid(start);
            ans += solve_centroid(centroid);
            removed[centroid] = 1;
            for (int v : adj[centroid])
            {
                if (removed[v])
                    continue;
                tasks.push_back(v);
            }
        }
        return ans;
    };

    long long ans = decompose();
    std::cout << ans << '\n';
    return 0;
}