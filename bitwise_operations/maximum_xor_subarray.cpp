#include <iostream>
#include <vector>

const int BITS = 30;

class BinaryTrie
{
public:
    explicit BinaryTrie(int sz)
    {
        trie.reserve(sz * BITS + 1);
        trie.emplace_back();
    }

    void insert(int x)
    {
        int p = 0;
        for (int bit = BITS - 1; bit >= 0; --bit)
        {
            int b = (x >> bit) & 1;
            if (trie[p].child[b] == -1)
            {
                trie[p].child[b] = trie.size();
                trie.emplace_back();
            }
            p = trie[p].child[b]; // either newly indexed node or existing prefix node
        }
    }

    int bestXor(int x)
    {
        int res = 0;
        int p = 0;
        for (int bit = BITS - 1; bit >= 0; --bit)
        {
            int b = (x >> bit) & 1;
            int want = b ^ 1;
            if (trie[p].child[want] != -1)
            {
                res |= (1 << bit);
                p = trie[p].child[want];
            }
            else
                p = trie[p].child[b];
        }
        return res;
    }

private:
    struct Node
    {
        std::vector<int> child;
        Node() { child.assign(2, -1); }
    };

    std::vector<Node> trie;
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int n;
    std::cin >> n;
    std::vector<int> array(n);
    for (auto &x : array)
        std::cin >> x;
    BinaryTrie btrie(n + 1);
    btrie.insert(0);
    int prefix = 0;
    int ans = 0;
    for (int x : array)
    {
        prefix ^= x;
        ans = std::max(ans, btrie.bestXor(prefix));
        btrie.insert(prefix);
    }
    std::cout << ans << '\n';
    return 0;
}