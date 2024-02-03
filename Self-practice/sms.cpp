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

    for (auto w : words) {
        for (int i = 0; i <= w.size() / 2; i++) {
            auto *node = root;
            for (char c : w[i]) {
                int pos = c - 'a';
                if (!node->children[pos]) node->children[pos] = new TrieNode();
                node = node->children[pos];
            }

            node->count = i + 1;
        }

        for (int i = w.size() - 1; i > w.size() / 2; i--) {
            auto *node = root;
            for (char c : w[i]) {
                int pos = c - 'a';
                if (!node->children[pos]) node->children[pos] = new TrieNode();
                node = node->children[pos];
            }

            node->count = i - w.size();
        }
    }

    int q;
    cin >> q;

    vector<tuple<long long, int, int>> dp(5e5 + 1);
    while (q--) {
        string s;
        cin >> s;

        dp[s.size()] = {0, 0, 0};
        for (int i = s.size() - 1; ~i; i--) {
            dp[i] = {1e18, 0, 0};
            
            auto *node = root;
            for (int j = i; j < s.size(); j++) {
                int pos = s[j] - 'a';
                if (!node->children[pos]) break;

                node = node->children[pos];
                if (node->count) {
                    int x = node->count < 0 ? -node->count : node->count - 1;
                    dp[i] = min(dp[i], {x + 1 + j + 1 - i + get<0>(dp[j + 1]), j + 1, node->count});
                }
            }
        }

        for (int i = 0; i < s.size();) {
            if (i) cout << "R";
            auto [_, j, presses] = dp[i];
            for (; i < j; i++) cout << mapping[s[i] - 'a'] + 2;
            int x = presses < 0 ? -presses : presses - 1;
            if (x) cout << (presses < 0 ? "D" : "U") << "(" << x << ")";
        }

        cout << "\n";
    }
}
