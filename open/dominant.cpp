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

        TrieNode(int range = 26) : next(range, -1) {}
    };

    vector<TrieNode> T;
    ascii a;
    int r;
    vector<int> ends;

    Trie(int n = 1, ascii alpha = LOWER, int range = 26) : T(n, TrieNode(range)), a(alpha), r(range) {}

    void add(const string &s) {
        int v = 0;
        for (char c : s) {
            int pos = c - a;

            if (!~T[v].next[pos]) {
                T.emplace_back(TrieNode(r));
                T[v].next[pos] = T.size() - 1;
            }
            v = T[v].next[pos];
        }
        ends.emplace_back(v);
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

    Trie trie;
    vector<string> sorted, S;
    string s;
    while (cin >> s) {
        S.emplace_back(s);
        sort(s.begin(), s.end());
        sorted.emplace_back(s);
        trie.add(s);
    }

    vector<bool> visited(trie.size(), false);
    auto dfs = [&](auto &&self, int i, int v, int end, string s) -> void {
        if (v != end) visited[v] = true;

        for (; i < s.size(); i++) {
            int pos = s[i] - 'a';
            if (~trie[v].next[pos]) self(self, i + 1, trie[v].next[pos], end, s);
        }
    };
    for (int i = 0; i < trie.ends.size(); i++) dfs(dfs, 0, 0, trie.ends[i], sorted[i]);

    vector<string> dominant;
    for (int i = 0; i < trie.ends.size(); i++)
        if (!visited[trie.ends[i]]) dominant.emplace_back(S[i]);
    sort(dominant.begin(), dominant.end());

    for (auto d : dominant) cout << d << "\n";
}
