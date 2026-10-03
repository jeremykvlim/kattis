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

    int add(const string &s) {
        int v = 0, steps = 0;
        for (char c : s) {
            int pos = c - a;

            if (!~T[v].next[pos]) {
                T.emplace_back(TrieNode(r));
                T[v].next[pos] = T.size() - 1;
            }
            v = T[v].next[pos];
            steps += ++T[v].count;
        }
        return steps;
    }

    int query(const string &s) {
        int v = 0, steps = 0;
        for (char c : s) {
            int pos = c - 'a';
            if (!~T[v].next[pos]) break;

            v = T[v].next[pos];
            steps += T[v].count;
        }
        return steps;
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
    unordered_map<string, int> database;
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        database[s] = i + trie.add(s);
    }

    int q;
    cin >> q;

    while (q--) {
        string s;
        cin >> s;

        if (database.count(s)) cout << database[s] + 1 << "\n";
        else cout << n + trie.query(s) << "\n";
    }
}
