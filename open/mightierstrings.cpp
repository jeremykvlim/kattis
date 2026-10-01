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

    int total_len = 0;
    vector<string> strings(n);
    for (auto &s : strings) {
        cin >> s;

        total_len += s.size();
    }

    int limit = 0;
    for (; (limit + 1) * (limit + 2) * (limit + 3) / 6 <= total_len && (limit + 1) * (limit + 2) / 2 <= n; limit++);

    Trie trie;
    for (auto &s : strings)
        if (s.size() <= limit) trie.add(s);

    vector<int> pref_node{0}, suff_node{0}, freq{0}, indices(trie.size(), -1);
    indices[0] = 0;
    for (auto &s : strings)
        if (s.size() <= limit) {
            int node = 0, pref = 0;
            for (char c : s) {
                pref = node;
                node = trie[node].next[c - 'a'];
            }

            if (!~indices[node]) {
                freq.emplace_back(0);
                indices[node] = freq.size() - 1;
                pref_node.emplace_back(pref);
                int suff = 0;
                for (int i = 1; i < s.size() && ~suff; i++) suff = trie[suff].next[s[i] - 'a'];
                suff_node.emplace_back(suff);
            }
            freq[indices[node]]++;
        }

    int m = freq.size();
    vector<vector<int>> adj_list(m);
    for (int u = 1; u < m; u++) {
        int v = indices[pref_node[u]];
        if (~v) adj_list[v].emplace_back(u);
    }

    int total = 0;
    vector<int> count(m, 0);
    stack<int> undo;
    auto dfs = [&](auto &&self, int v) -> void {
        bool mighty = true;
        int version = undo.size();

        for (int u = v; u; u = indices[suff_node[u]]) {
            count[u]++;
            undo.emplace(u);

            if (count[u] > freq[u] || !~suff_node[u] || !~indices[suff_node[u]]) {
                mighty = false;
                break;
            }
        }

        if (mighty) {
            total += freq[v];
            for (int u : adj_list[v]) self(self, u);
        }

        while (undo.size() > version) {
            count[undo.top()]--;
            undo.pop();
        }
    };
    for (int v : adj_list[0]) dfs(dfs, v);
    cout << total;
}