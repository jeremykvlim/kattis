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
        int count;

        TrieNode(int range = 26) : next(range, -1), count(0) {}
    };

    vector<TrieNode> T;
    ascii a;
    int r;

    Trie(int n = 1, ascii alpha = LOWER, int range = 26) : T(n, TrieNode(range)), a(alpha), r(range) {}

    void add(const string &s, int count) {
        int v = 0;
        for (char c : s) {
            int pos = c - a;

            if (!~T[v].next[pos]) {
                T.emplace_back(TrieNode(r));
                T[v].next[pos] = T.size() - 1;
            }
            v = T[v].next[pos];
        }
        T[v].count = count;
    }

    int query(const string &s) {
        int n = s.size(), count = 1;
        auto dfs = [&](auto &&self, int i = 0, int v = 0, bool edit = true) -> void {
            if (i == n) {
                if (T[v].count) count = max(count, T[v].count + 1);
                return;
            }

            int pos = s[i] - a;
            if (~T[v].next[pos]) self(self, i + 1, T[v].next[pos], edit);

            if (edit) {
                self(self, i + 1, v, false);
                for (int p = 0; p < r; p++) {
                    int u = T[v].next[p];
                    if (!~u) continue;

                    self(self, i, u, false);
                    if (p != pos) self(self, i + 1, u, false);
                }
            }
        };
        dfs(dfs);
        return count;
    }

    auto & operator[](int i) {
        return T[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int count = 0;
    Trie trie;
    string s;
    while (cin >> s) {
        int c = trie.query(s);
        trie.add(s, c);
        count = max(count, c);
    }
    cout << count;
}