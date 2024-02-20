#include <bits/stdc++.h>
using namespace std;

struct TrieNode {
    TrieNode *children[26] = {nullptr};
    int count = 0;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> mapping{0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 7, 7, 7, 7};
    auto *root = new TrieNode();
    vector<vector<string>> words;
    int count = 0;
    while (n--) {
        string s;
        cin >> s;

        auto *node = root;
        for (char c : s) {
            int pos = mapping[c - 'a'];
            if (!node->children[pos]) node->children[pos] = new TrieNode();
            node = node->children[pos];
        }

        if (!node->count) {
            node->count = ++count;
            words.emplace_back(vector<string>());
        }
        words[node->count - 1].emplace_back(s);
    }

    for (int i = 0; i < count; i++) {
        for (int j = 0; j <= words[i].size() / 2; j++) {
            auto *node = root;
            for (char c : words[i][j]) {
                int pos = c - 'a';
                if (!node->children[pos]) node->children[pos] = new TrieNode();
                node = node->children[pos];
            }
            node->count = j + 1;
        }

        for (int j = words[i].size() - 1; j > words[i].size() / 2; j--) {
            auto *node = root;
            for (char c : words[i][j]) {
                int pos = c - 'a';
                if (!node->children[pos]) node->children[pos] = new TrieNode();
                node = node->children[pos];
            }
            node->count = j - words[i].size();
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
            
            auto *node = root;
            for (int j = i; j < w.size(); j++) {
                int pos = w[j] - 'a';
                if (!node->children[pos]) break;

                node = node->children[pos];
                if (node->count) {
                    int c = node->count < 0 ? -node->count : node->count - 1;
                    dp[i] = min(dp[i], {c + 1 + j + 1 - i + get<0>(dp[j + 1]), j + 1, node->count});
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
