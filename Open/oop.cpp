#include <bits/stdc++.h>
using namespace std;

struct TrieNode {
    TrieNode *children[26]{nullptr};
};

void dfs(unordered_set<long long> &hashes, map<long long, vector<int>> &shared, unordered_map<TrieNode *, vector<long long>> &word_hash, unordered_map<TrieNode *, int> &start, unordered_map<TrieNode *, int> &end, TrieNode *curr, int &count) {
    start[curr] = ++count;

    for (auto &h : word_hash[curr])
        if (hashes.count(h)) shared[h].emplace_back(start[curr]);

    for (auto &c : curr->children)
        if (c) dfs(hashes, shared, word_hash, start, end, c, count);

    end[curr] = ++count;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<string> words(n);
    int longest = 0;
    for (auto &w : words) {
        cin >> w;
        
        longest = max(longest, (int) w.size());
    }

    auto *root = new TrieNode();
    unordered_map<TrieNode *, vector<long long>> word_hash;
    vector<long long> hash(longest + 1, 0);
    long long b = 1e16 + 61, mod = (1LL << 62) + 135;
    for (auto w : words) {
        hash[w.size()] = 0;
        for (int i = w.size() - 1; ~i; i--)
            hash[i] = (hash[i + 1] * b % mod + w[i] - 'a' + 1) % mod;

        auto *node = root;
        word_hash[node].emplace_back(hash[0]);
        for (int i = 0; i < w.size(); i++) {
            int pos = w[i] - 'a';
            if (!node->children[pos]) node->children[pos] = new TrieNode();
            node = node->children[pos];

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

    unordered_map<TrieNode *, int> start, end;
    map<long long, vector<int>> shared;
    int count = 0;
    dfs(hashes, shared, word_hash, start, end, root, count);

    for (int i = 0; i < q; i++) {
        auto *node = root;
        for (char c : pref[i]) {
            int pos = c - 'a';
            if (!node->children[pos]) {
                cout << "0\n";
                goto next;
            }
            node = node->children[pos];
        }

        cout << upper_bound(shared[suff_hash[i]].begin(), shared[suff_hash[i]].end(), end[node]) - lower_bound(shared[suff_hash[i]].begin(), shared[suff_hash[i]].end(), start[node]) << "\n";
        next:;
    }
}
