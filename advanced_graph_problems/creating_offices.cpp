#include <iostream>
#include <vector>
#include <queue>

const int INF = 2e5;
const int MAXN = 2e5;
const int LOG = 20; // each node has around log(n) <= log(2e5) < 1 + 5 * 4 = 21

// tree as a linked list
int head[MAXN];
int to[MAXN * 2];
int next[MAXN * 2];
int edgeCnt = 0;

// centroid decomposition
int component[MAXN];
int parent[MAXN];
int subtree[MAXN];
unsigned char removed[MAXN];

// centroid path
int centroids[MAXN][LOG];
int centroidDists[MAXN][LOG];
unsigned char centroidCount[MAXN]; // Max # of centroids = 20; enough for 8-bit
int best[MAXN];                    // best[c] = minimum distance to centroid c from any selected node

// sratch arrays when iteratively building centroid path
int stNode[MAXN];
int stParent[MAXN];
int stDist[MAXN];

void add_edge(int u, int v)
{
    to[edgeCnt] = v;
    next[edgeCnt] = head[u];
    head[u] = edgeCnt++;
}

int find_centroid(int start)
{
    // BFS build component
    int sz = 0;
    parent[start] = -1;
    subtree[start] = 1;
    component[sz++] = start;
    for (int i = 0; i < sz; ++i)
    {
        int u = component[i];
        for (int e = head[u]; e != -1; e = next[e])
        {
            int v = to[e];
            if (removed[v] || parent[u] == v)
                continue;
            parent[v] = u;
            subtree[v] = 1;
            component[sz++] = v;
        }
    }

    // Calculate subtree size
    for (int i = sz - 1; i > 0; --i)
    {
        int u = component[i];
        subtree[parent[u]] += subtree[u];
    }

    // Find centroid
    int u = start;
    while (true)
    {
        int heavyChild = -1;
        for (int e = head[u]; e != -1; e = next[e])
        {
            int v = to[e];
            if (removed[v] || parent[v] != u)
                continue;
            if (2 * subtree[v] > sz)
            {
                heavyChild = v;
                break;
            }
        }
        if (heavyChild == -1)
            return u;
        u = heavyChild;
    }
}

void build_centroid(int start)
{
    int centroid = find_centroid(start);
    int top = 0;
    stNode[top] = centroid;
    stParent[top] = -1;
    stDist[top] = 0;
    for (int i = 0; i <= top; ++i)
    {
        int u = stNode[i];
        int par = stParent[i];
        int dist = stDist[i];
        int p = centroidCount[u]++;
        centroids[u][p] = centroid;
        centroidDists[u][p] = dist;
        for (int e = head[u]; e != -1; e = next[e])
        {
            int v = to[e];
            if (removed[v] || v == par)
                continue;
            stNode[++top] = v;
            stParent[top] = u;
            stDist[top] = dist + 1;
        }
    }
    removed[centroid] = 1;
    for (int e = head[centroid]; e != -1; e = next[e])
    {
        int v = to[e];
        if (!removed[v])
            build_centroid(v);
    }
}

bool canCreateOffice(int u, int d)
{
    // post: checks if distance to the nearest node already selected is at least d
    for (int i = 0; i < (int)centroidCount[u]; ++i)
    {
        int centroid = centroids[u][i];
        int dist = centroidDists[u][i];
        if (best[centroid] + dist < d)
            return false;
        // minDist = std::min(best[centroid] + dist, minDist);
    }
    return true;
}

void createOffice(int u)
{
    for (int i = 0; i < (int)centroidCount[u]; ++i)
    {
        int centroid = centroids[u][i];
        int dist = centroidDists[u][i];
        best[centroid] = std::min(dist, best[centroid]);
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n, d;
    std::cin >> n >> d;

    std::fill(head, head + n, -1);
    std::fill(best, best + n, INF);

    for (int i = 0; i < n - 1; ++i)
    {
        int a, b;
        std::cin >> a >> b;
        --a;
        --b;
        add_edge(a, b);
        add_edge(b, a);
    }

    build_centroid(0);

    int top = 0;
    stNode[top++] = 0;
    parent[0] = -1;
    for (int i = 0; i < top; ++i)
    {
        int u = stNode[i];
        for (int e = head[u]; e != -1; e = next[e])
        {
            int v = to[e];
            if (parent[u] == v)
                continue;
            parent[v] = u;
            stNode[top++] = v;
        }
    }

    std::vector<int> offices;
    for (int i = top - 1; i >= 0; --i)
    {
        int u = stNode[i];
        if (canCreateOffice(u, d))
        {
            createOffice(u);
            offices.push_back(u);
        }
    }
    std::cout << offices.size() << '\n';
    for (auto &u : offices)
        std::cout << u + 1 << " ";
    return 0;
}
