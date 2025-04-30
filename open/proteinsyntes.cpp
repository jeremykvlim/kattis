#include <bits/stdc++.h>
using namespace std;

struct Trie {
    enum ascii {
        LOWER = 97,
        UPPER = 65,
        NUM = 48,
        SYM = 32,
        NA = 0
    };

    struct TrieNode {
        vector<int> next;
        int link;

        TrieNode(int range = 26) : next(range, -1), link(-1) {}
    };

    vector<TrieNode> T;
    ascii a;
    int r, count;
    vector<int> jump_link, head, jump_index, len;

    Trie(int n = 1, ascii alpha = LOWER, int range = 26) : T(n, TrieNode(range)), a(alpha), r(range), count(0), jump_link{-1}, head{-1} {}

    void add(string &s) {
        int node = 0;
        for (char ch : s) {
            int pos = ch - a;

            if (T[node].next[pos] == -1) {
                T[node].next[pos] = T.size();
                T.emplace_back(TrieNode(r));
                jump_link.emplace_back(-1);
                head.emplace_back(-1);
            }
            node = T[node].next[pos];
        }

        if (head[node] == -1) {
            len.emplace_back(s.size());
            jump_index.emplace_back(-1);
            head[node] = count++;
        }
    }

    void aho_corasick() {
        queue<int> q;
        q.emplace(0);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            int l = T[v].link;
            for (int c = 0; c < r; c++) {
                int u = T[v].next[c];

                if (u != -1) {
                    T[u].link = (l == -1) ? 0 : T[l].next[c];
                    jump_link[u] = (head[T[u].link] != -1) ? T[u].link : jump_link[T[u].link];
                    q.emplace(u);
                } else T[v].next[c] = (l == -1) ? 0 : T[l].next[c];
            }
        }
        T[0].link = 0;
    }

    auto & operator[](int i) {
        return T[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    string t;
    cin >> n >> t >> m;

    Trie trie;
    while (m--) {
        string s;
        cin >> s;

        trie.add(s);
    }
    trie.aho_corasick();

    vector<int> dp(n + 1, 1e9);
    dp[0] = 0;
    int node = 0;
    for (int i = 1; i <= n; i++) {
        int c = t[i - 1] - 'a';
        node = trie[node].next[c];
        for (int v = node; v != -1; v = trie.jump_link[v])
            for (int j = trie.head[v]; j != -1; j = trie.jump_index[j]) dp[i] = min(dp[i], dp[i - trie.len[j]] + 1);
    }
    cout << dp[n];
}
