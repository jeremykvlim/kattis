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
    vector<string> strings(n);
    for (auto &s : strings) {
        cin >> s;

        trie.add(s);
    }

    vector<int> pref_node{0}, suff_node{0}, freq{0}, indices(trie.size(), -1);
    indices[0] = 0;
    for (auto &s : strings) {
        int v = 0, pref = 0;
        for (char c : s) {
            pref = v;
            v = trie[v].next[c - 'a'];
        }

        if (!~indices[v]) {
            freq.emplace_back(0);
            indices[v] = freq.size() - 1;
            pref_node.emplace_back(pref);
            int suff = 0;
            for (int i = 1; i < s.size() && ~suff; i++) suff = trie[suff].next[s[i] - 'a'];
            suff_node.emplace_back(suff);
        }
        freq[indices[v]]++;
    }

    int m = freq.size();
    vector<vector<int>> adj_list(m);
    for (int u = 1; u < m; u++) {
        int v = indices[pref_node[u]];
        if (~v) adj_list[v].emplace_back(u);
    }

    int total = 0;
    auto dfs = [&](auto &&self, int v) -> void {
        bool mighty = true;
        for (int u = v; u; u = indices[suff_node[u]])
            if (!~suff_node[u] || !~indices[suff_node[u]]) {
                mighty = false;
                break;
            }

        if (mighty) {
            total += freq[v];
            for (int u : adj_list[v]) self(self, u);
        }
    };
    for (int v : adj_list[0]) dfs(dfs, v);
    cout << total;
}