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

    void add(string &s, int &steps) {
        int node = 0;
        for (char c : s) {
            int pos = c - a;

            if (T[node].next[pos] == -1) {
                T[node].next[pos] = T.size();
                T.emplace_back(TrieNode(r));
            }
            node = T[node].next[pos];
            steps += ++T[node].count;
        }
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

    unordered_map<string, int> database;
    Trie trie;
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        int steps = i;
        trie.add(s, steps);
        database[s] = steps;
    }

    int q;
    cin >> q;

    while (q--) {
        string s;
        cin >> s;

        if (database.count(s)) cout << database[s] + 1 << "\n";
        else {
            int steps = n, node = 0;
            for (char c : s) {
                int pos = c - 'a';
                if (trie[node].next[pos] == -1) break;

                node = trie[node].next[pos];
                steps += trie[node].count;
            }

            cout << steps << "\n";
        }
    }
}
