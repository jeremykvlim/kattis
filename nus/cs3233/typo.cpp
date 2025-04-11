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

    void add(string &s) {
        int node = 0;
        for (char c : s) {
            int pos = c - a;

            if (T[node].next[pos] == -1) {
                T[node].next[pos] = T.size();
                T.emplace_back(TrieNode(r));
            }
            node = T[node].next[pos];
        }
        T[node].count++;
    }

    bool typo(string &s) {
        int n = s.size();

        auto dfs = [&](auto &&self, int node = 0, int i = 0, bool deleted = false) {
            if (i == n) return deleted && T[node].count;

            if (!deleted) {
                if (self(self, node, i + 1, true)) return true;
            }

            char c = s[i];
            int pos = c - a;

            if (T[node].next[pos] == -1) return false;
            return self(self, T[node].next[pos], i + 1, deleted);
        };
        return dfs(dfs);
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
    vector<string> dict(n);
    for (auto &word : dict) {
        cin >> word;

        trie.add(word);
    }

    vector<string> typos;
    for (auto word : dict)
        if (trie.typo(word)) typos.emplace_back(word);

    if (typos.empty()) cout << "NO TYPOS" << "\n";
    else
        for (auto word : typos) cout << word << "\n";
}
