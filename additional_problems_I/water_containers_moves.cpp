#include <iostream>
#include <queue>
#include <algorithm>
#include <limits>

typedef long long int64;

const int64 INF = std::numeric_limits<int64>::max() / 4;
const std::string MOVES[6] = {"FILL A", "EMPTY A", "FILL B", "EMPTY B", "MOVE A B", "MOVE B A"};

struct State
{
    int64 dist;
    int id;
    bool operator>(const State &other) const { return dist > other.dist; }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int a, b, x;
    std::cin >> a >> b >> x;
    if (a < x)
    {
        std::cout << -1 << '\n';
        return 0;
    }
    int width = b + 1;
    auto getId = [&](int A, int B)
    { return A * width + B; };
    int nStates = (a + 1) * (b + 1);
    std::vector<int64> dist(nStates, INF);
    std::vector<int> parent(nStates, -1), parentMove(nStates);
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;
    dist[0] = 0;
    pq.push({0, 0});
    int goal = -1;
    while (!pq.empty())
    {
        State cur = pq.top();
        pq.pop();
        int64 d = cur.dist;
        int id = cur.id;
        if (d != dist[id])
            continue;
        int A = id / width;
        int B = id % width;
        if (A == x)
        {
            goal = id;
            break;
        }
        auto transition = [&](int nextA, int nextB, int amount, int move)
        {
            int64 newDist = (int64)(d + amount);
            int newId = getId(nextA, nextB);
            if (newDist < dist[newId])
            {
                dist[newId] = newDist;
                parent[newId] = id;
                parentMove[newId] = move;
                pq.push({newDist, newId});
            }
        };
        if (A < a)
            transition(a, B, a - A, 0);
        if (A > 0)
            transition(0, B, A, 1);
        if (B < b)
            transition(A, b, b - B, 2);
        if (B > 0)
            transition(A, 0, B, 3);
        int a2b = std::min(A, b - B);
        if (a2b > 0)
            transition(A - a2b, B + a2b, a2b, 4);
        int b2a = std::min(a - A, B);
        if (b2a > 0)
            transition(A + b2a, B - b2a, b2a, 5);
    }
    if (goal == -1)
    {
        std::cout << -1 << '\n';
        return 0;
    }
    std::vector<int> seq;
    for (int s = goal; s != 0; s = parent[s])
        seq.push_back(parentMove[s]);
    std::reverse(seq.begin(), seq.end());
    std::cout << seq.size() << " " << dist[goal] << '\n';
    for (int mv : seq)
        std::cout << MOVES[mv] << '\n';
    return 0;
}