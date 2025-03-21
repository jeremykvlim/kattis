#include <bits/stdc++.h>
using namespace std;

struct Trie {
    enum ascii {
        LOWER = 97,
        UPPER = 65,
        NUM = 48,
        NA = 0
    };

    struct TrieNode {
        vector<int> next;
        array<int, 3> queries;

        TrieNode(int range = 26) : next(range, -1), queries({0, 0, 0}) {}
    };

    vector<TrieNode> T;
    ascii a;
    int r;
    vector<int> order;

    Trie(int n = 1, ascii alpha = LOWER, int range = 26) : T(n, TrieNode(range)), a(alpha), r(range) {}

    int add(string &s, int q) {
        int node = 0;
        if (!q) {
            for (int i = 0; i < s.size(); i++) {
                int pos = s[i] - a;

                if (T[node].next[pos] == -1) {
                    T[node].next[pos] = T.size();
                    T.emplace_back(TrieNode(r));
                }
                node = T[node].next[pos];
                if (i & 1) T[node].queries[q]++;
            }
        } else {
            for (char c : s) {
                int pos = c - a;

                if (T[node].next[pos] == -1) {
                    T[node].next[pos] = T.size();
                    T.emplace_back(TrieNode(r));
                }
                node = T[node].next[pos];
                T[node].queries[q]++;
            }
        }

        return node;
    }

    int query(string &s, int q) {
        int node = 0;
        for (char c : s) {
            int pos = c - a;

            if (T[node].next[pos] == -1) {
                T[node].next[pos] = T.size();
                T.emplace_back(TrieNode(r));
            }
            node = T[node].next[pos];
        }

        return T[node].queries[q];
    }

    auto & operator[](int i) {
        return T[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    Trie trie;
    vector<string> dict(n);
    for (auto &w : dict) {
        cin >> w;

        auto rev = w;
        reverse(rev.begin(), rev.end());

        string both(2 * w.size(), '#');
        for (int i = 0; i < w.size(); i++) {
            both[2 * i] = w[i];
            both[2 * i + 1] = rev[i];
        }
        trie.add(both, 0);
        trie.add(w, 1);
        trie.add(rev, 2);
    }

    while (q--) {
        string o, p, s;
        cin >> o >> p >> s;

        auto rev = s;
        reverse(rev.begin(), rev.end());

        string both(2 * p.size(), '#');
        for (int i = 0; i < p.size(); i++) {
            both[2 * i] = p[i];
            both[2 * i + 1] = rev[i];
        }

        if (o == "AND") cout << trie.query(both, 0) << "\n";
        else cout << trie.query(p, 1) + trie.query(rev, 2) - (o == "OR" ? 1 : 2) * trie.query(both, 0) << "\n";
    }
}
