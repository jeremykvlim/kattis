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

    unordered_map<string, int> database;
    auto *root = new TrieNode();
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        int steps = i;
        auto *node = root;
        for (char c : s) {
            int pos = c - 'a';
            if (!node->children[pos]) node->children[pos] = new TrieNode();
            node = node->children[pos];
            steps += ++node->count;
        }

        database[s] = steps;
    }

    int q;
    cin >> q;

    while (q--) {
        string s;
        cin >> s;
        
        if (database.count(s)) cout << database[s] + 1 << "\n";
        else {
            int steps = n;
            auto *node = root;
            for (char c : s) {
                int pos = c - 'a';
                if (!node->children[pos]) break;
                node = node->children[pos];
                steps += node->count;
            }

            cout << steps << "\n";
        }
    }
}
