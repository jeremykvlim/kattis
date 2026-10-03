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
        int depth, count, link;

        TrieNode(int range = 26, int d = 0) : next(range, -1), depth(d), count(0), link(-1) {}
    };

    vector<TrieNode> T;
    ascii a;
    int r;

    Trie(int n = 1, ascii alpha = LOWER, int range = 26) : T(n, TrieNode(range)), a(alpha), r(range) {}

    int add(const string &s) {
        int v = 0;
        for (char c : s) {
            int pos = c - a;

            if (!~T[v].next[pos]) {
                T.emplace_back(TrieNode(r, T[v].depth + 1));
                T[v].next[pos] = T.size() - 1;
            }
            v = T[v].next[pos];
        }

        return v;
    }

    vector<int> aho_corasick() {
        vector<int> order;
        queue<int> q;
        q.emplace(0);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            order.emplace_back(v);

            int l = T[v].link;
            for (int c = 0; c < r; c++) {
                int u = T[v].next[c];

                if (~u) {
                    T[u].link = !~l ? 0 : T[l].next[c];
                    q.emplace(u);
                } else T[v].next[c] = !~l ? 0 : T[l].next[c];
            }
        }
        T[0].link = 0;
        return order;
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

    vector<string> words(n), patterns(q);
    for (auto &w : words) {
        cin >> w;

        w = w + '{' + w;
    }

    Trie trie(1, Trie::ascii::LOWER, 27);
    vector<int> nodes(q);
    for (int &v : nodes) {
        string p;
        cin >> p;

        string pref, suff;
        for (char c : p) {
            if (c == '*') {
                swap(pref, suff);
                continue;
            }
            pref += c;
        }
        p = pref + '{' + suff;
        v = trie.add(p);
    }

    auto order = trie.aho_corasick();
    reverse(order.begin(), order.end());
    for (const auto &s : words) {
        int v = 0;
        for (char c : s) {
            int pos = c - 'a';

            v = trie[v].next[pos];
            while (trie[v].depth > s.size() / 2 + 1) v = trie[v].link;
            trie[v].count++;
        }
    }
    for (int v : order) trie[trie[v].link].count += trie[v].count;
    for (int v : nodes) cout << trie[v].count << "\n";
}
