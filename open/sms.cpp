#include <bits/stdc++.h>
using namespace std;

struct Trie {
    struct TrieNode {
        vector<int> next;
        int count = 0;

        TrieNode() {
            next.resize(26, -1);
        }
    };

    vector<TrieNode> T;

    Trie(int n = 1) : T(n) {}

    void add(string &s, int count) {
        int node = 0;
        for (char c : s) {
            int pos = c - 'a';

            if (T[node].next[pos] == -1) {
                T[node].next[pos] = T.size();
                T.emplace_back();
            }
            node = T[node].next[pos];
        }

        T[node].count = count;
    }

    void add(string &s, vector<int> mapping, vector<vector<string>> &words, int &count) {
        int node = 0;
        for (char c : s) {
            int pos = mapping[c - 'a'];

            if (T[node].next[pos] == -1) {
                T[node].next[pos] = T.size();
                T.emplace_back();
            }
            node = T[node].next[pos];
        }

        if (!T[node].count) {
            T[node].count = ++count;
            words.emplace_back(vector<string>());
        }

        words[T[node].count - 1].emplace_back(s);
    }

    auto & operator[](int i) {
        return T[i];
    }
};

struct TrieNode {
    vector<int> next;
    int count = 0;

    TrieNode() {
        next.resize(26, -1);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> mapping{0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 7, 7, 7, 7};
    Trie trie;
    vector<vector<string>> words;
    int count = 0;
    while (n--) {
        string s;
        cin >> s;

        trie.add(s, mapping, words, count);
    }

    for (int i = 0; i < count; i++) {
        for (int j = 0; j <= words[i].size() / 2; j++)
            trie.add(words[i][j], j + 1);

        for (int j = words[i].size() - 1; j > words[i].size() / 2; j--)
            trie.add(words[i][j], j - words[i].size());
    }

    int q;
    cin >> q;

    vector<string> queries(q);
    int longest = 0;
    for (auto &w : queries) {
        cin >> w;

        longest = max(longest, (int) w.size());
    }

    vector<tuple<long long, int, int>> dp(longest + 1, {0, 0, 0});
    for (auto w : queries) {
        for (int i = w.size() - 1; ~i; i--) {
            dp[i] = {1e18, 0, 0};

            int node = 0;
            for (int j = i; j < w.size(); j++) {
                int pos = w[j] - 'a';
                if (trie[node].next[pos] == -1) break;

                node = trie[node].next[pos];
                if (trie[node].count) {
                    int c = trie[node].count < 0 ? -trie[node].count : trie[node].count - 1;
                    dp[i] = min(dp[i], {c + 1 + j + 1 - i + get<0>(dp[j + 1]), j + 1, trie[node].count});
                }
            }
        }

        for (int i = 0; i < w.size();) {
            auto [_, len, x] = dp[i];

            if (i) cout << "R";
            for (; i < len; i++) cout << mapping[w[i] - 'a'] + 2;
            if (x < 0) cout << "D(" << -x << ")";
            if (x > 1) cout << "U(" << x - 1 << ")";
        }
        cout << "\n";
    }
}
