#include <bits/stdc++.h>
using namespace std;

struct Trie {
    struct TrieNode {
        vector<int> next;

        TrieNode() {
            next.resize(26, -1);
        }
    };

    vector<TrieNode> T;

    Trie() : T(1) {}

    void add(string &s) {
        int node = 0;
        for (char c : s) {
            int pos = c - 'a';

            if (T[node].next[pos] == -1) {
                T[node].next[pos] = T.size();
                T.emplace_back();
            }
            node = T[node].next[pos];
        }
    }

    auto operator[](int i) {
        return T[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    Trie trie;
    vector<string> words(n);
    int longest = 0;
    for (auto &w : words) {
        cin >> w;

        trie.add(w);
        longest = max(longest, (int) w.size());
    }

    unordered_map<int, vector<long long>> word_hash;
    vector<long long> hash(longest + 1, 0);
    auto b = (long long) 1e16 + 61, mod = (1LL << 62) + 135;
    for (auto w : words) {
        hash[w.size()] = 0;
        for (int i = w.size() - 1; ~i; i--)
            hash[i] = (hash[i + 1] * b % mod + w[i] - 'a' + 1) % mod;

        int node = 0;
        word_hash[node].emplace_back(hash[0]);
        for (int i = 0; i < w.size(); i++) {
            int pos = w[i] - 'a';
            node = trie[node].next[pos];
            word_hash[node].emplace_back(hash[i + 1]);
        }
    }

    vector<string> pref(q);
    vector<long long> suff_hash(q, 0);
    unordered_set<long long> hashes;
    for (int i = 0; i < q; i++) {
        string s;
        cin >> s;

        pref[i] = s.substr(0, s.find('*'));
        auto suff = s.substr(s.find('*') + 1);
        reverse(suff.begin(), suff.end());

        for (char c : suff)
            suff_hash[i] = (suff_hash[i] * b % mod + c - 'a' + 1) % mod;

        hashes.emplace(suff_hash[i]);
    }

    unordered_map<int, int> start, end;
    map<long long, vector<int>> shared;
    int count = 0;
    auto dfs = [&](auto &&self, int curr) -> void {
        start[curr] = ++count;

        for (auto h : word_hash[curr])
            if (hashes.count(h)) shared[h].emplace_back(start[curr]);

        for (int c : trie[curr].next)
            if (c != -1) self(self, c);

        end[curr] = ++count;
    };

    dfs(dfs, 0);

    for (int i = 0; i < q; i++) {
        int node = 0;
        for (char c : pref[i]) {
            int pos = c - 'a';
            if (trie[node].next[pos] == -1) {
                cout << "0\n";
                goto next;
            }
            node = trie[node].next[pos];
        }

        cout << upper_bound(shared[suff_hash[i]].begin(), shared[suff_hash[i]].end(), end[node]) -
                lower_bound(shared[suff_hash[i]].begin(), shared[suff_hash[i]].end(), start[node]) << "\n";
        next:;
    }
}
