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
        int link, end_link;

        TrieNode(int range = 26) : next(range, -1), link(-1), end_link(-1) {}
    };

    vector<TrieNode> T;
    ascii a;
    int r;

    Trie(int n = 1, ascii alpha = LOWER, int range = 26) : T(n, TrieNode(range)), a(alpha), r(range) {}

    int add(string &s) {
        int node = 0;
        for (char c : s) {
            int pos = c - a;

            if (T[node].next[pos] == -1) {
                T[node].next[pos] = T.size();
                T.emplace_back(TrieNode(r));
            }
            node = T[node].next[pos];
        }
        T[node].end_link = node;

        return node;
    }

    void aho_corasick() {
        queue<int> q;
        q.emplace(0);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            int l = T[v].link;
            if (l != -1 && T[v].end_link == -1) T[v].end_link = T[l].end_link;
            for (int c = 0; c < r; c++) {
                int u = T[v].next[c];

                if (u != -1) {
                    T[u].link = (l == -1) ? 0 : T[l].next[c];
                    q.emplace(u);
                } else T[v].next[c] = (l == -1) ? 0 : T[l].next[c];
            }
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

                node = trie[node].next[pos];
                for (int v = trie[node].end_link; ~v; v = trie[trie[v].link].end_link)
                    if (visited[v] != q) {
                        visited[v] = q;
                        count[v]++;
                    } else break;
            }
        } else {
            int s;
            cin >> s;
            cout << count[nodes[s]] << "\n";
        }
    }
}
