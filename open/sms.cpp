#include <bits/stdc++.h>
using namespace std;

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
    vector<TrieNode> trie;
    trie.emplace_back();
    vector<vector<string>> words;
    int count = 0;
    while (n--) {
        string s;
        cin >> s;

        int node = 0;
        for (char c : s) {
            int pos = mapping[c - 'a'];

            if (trie[node].next[pos] == -1) {
                trie[node].next[pos] = trie.size();
                trie.emplace_back();
            }

            node = trie[node].next[pos];
        }
        if (!trie[node].count) {
            trie[node].count = ++count;
            words.emplace_back(vector<string>());
        }

        words[trie[node].count - 1].emplace_back(s);
    }

    for (int i = 0; i < count; i++) {
        for (int j = 0; j <= words[i].size() / 2; j++) {
            int node = 0;
            for (char c : words[i][j]) {
                int pos = c - 'a';

                if (trie[node].next[pos] == -1) {
                    trie[node].next[pos] = trie.size();
                    trie.emplace_back();
                }

                node = trie[node].next[pos];
            }
            trie[node].count = j + 1;
        }

        for (int j = words[i].size() - 1; j > words[i].size() / 2; j--) {
            int node = 0;
            for (char c : words[i][j]) {
                int pos = c - 'a';

                if (trie[node].next[pos] == -1) {
                    trie[node].next[pos] = trie.size();
                    trie.emplace_back();
                }

                node = trie[node].next[pos];
            }
            trie[node].count = j - words[i].size();
        }
    }

    int q;
    cin >> q;

    vector<string> type(q);
    int longest = 0;
    for (auto &w : type) {
        cin >> w;

        longest = max(longest, (int) w.size());
    }

    vector<tuple<long long, int, int>> dp(longest + 1, {0, 0, 0});
    for (auto w : type) {
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
            if (i) cout << "R";
            auto [_, len, presses] = dp[i];
            for (; i < len; i++) cout << mapping[w[i] - 'a'] + 2;
            int x = presses < 0 ? -presses : presses - 1;
            if (x) cout << (presses < 0 ? "D" : "U") << "(" << x << ")";
        }

        cout << "\n";
    }
}
