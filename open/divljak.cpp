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
        bool end;

        TrieNode(int range = 26) : next(range, -1), link(-1), end(false) {}
    };

    vector<TrieNode> T;
    ascii a;
    int r;
    vector<int> end_link;

    Trie(int n = 1, ascii alpha = LOWER, int range = 26) : T(n, TrieNode(range)), a(alpha), r(range), end_link{-1} {}

    int add(string &s) {
        int node = 0;
        for (char c : s) {
            int pos = c - a;

            if (T[node].next[pos] == -1) {
                T[node].next[pos] = T.size();
                T.emplace_back(TrieNode(r));
                end_link.emplace_back(-1);
            }
            node = T[node].next[pos];
        }
        T[node].end = true;

        return node;
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
                    q.emplace(u);
                } else T[v].next[c] = (l == -1) ? 0 : T[l].next[c];
            }

            if (T[v].end) end_link[v] = v;
            else if (v) end_link[v] = end_link[T[v].link];
        }
        T[0].link = 0;
    }

    int size() {
        return T.size();
    }

    auto & operator[](int i) {
        return T[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    Trie trie;
    vector<int> nodes(n + 1);
    for (int i = 1; i <= n; i++) {
        string s;
        cin >> s;

        nodes[i] = trie.add(s);
    }
    trie.aho_corasick();

    int q;
    cin >> q;

    vector<int> count(trie.size(), 0), visited(trie.size(), -1);
    while (q--) {
        int t;
        cin >> t;

        if (t == 1) {
            string s;
            cin >> s;

            int node = 0;
            for (char c : s) {
                int pos = c - 'a';

                for (; !~trie[node].next[pos]; node = trie[node].link);
                node = trie[node].next[pos];

                int v = trie.end_link[node];
                while (~v && visited[v] != q) {
                    count[v]++;
                    visited[v] = q;
                    v = trie.end_link[trie[v].link];
                }
            }
        } else {
            int s;
            cin >> s;
            cout << count[nodes[s]] << "\n";
        }
    }
}
