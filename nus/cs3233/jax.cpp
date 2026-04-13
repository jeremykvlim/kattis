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
            T[node].count++;
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

    vector<string> s(n);
    vector<int> offset(n + 1, 0);
    for (int i = 0; i < n; i++) {
        cin >> s[i];

        offset[i + 1] = offset[i] + s[i].size();
    }

    Trie pref_trie, suff_trie;
    for (int i = 0; i < n; i++) {
        pref_trie.add(s[i]);
        auto rev = s[i];
        reverse(rev.begin(), rev.end());
        suff_trie.add(rev);
    }

    int total = offset[n];
    vector<int> pref(total, 0), suff(total, 0);
    for (int i = 0; i < n; i++) {
        for (int j = 0, node = 0; j < s[i].size(); j++) node = pref[offset[i] + j] = pref_trie[node].next[s[i][j] - 'a'];
        for (int j = 0, node = 0; j < s[i].size(); j++) node = suff[offset[i] + j] = suff_trie[node].next[s[i][s[i].size() - 1 - j] - 'a'];
    }

    vector<pair<int, int>> pairs;
    for (int i = 0; i < total; i++) pairs.emplace_back(pref[i], suff[i]);
    sort(pairs.begin(), pairs.end());

    for (int i = 0; i < n; i++) {
        int k = 0;
        for (int j = s[i].size(); j; j--) {
            auto p = make_pair(pref[offset[i] + j - 1], suff[offset[i] + j - 1]);
            if (pref_trie[p.first].count < 2 || suff_trie[p.second].count < 2) continue;

            auto [l, r] = equal_range(pairs.begin(), pairs.end(), p);
            if (l + 1 == r) {
                k = j;
                break;
            }
        }
        cout << k << " ";
    }
}