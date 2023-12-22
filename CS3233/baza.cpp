#include <bits/stdc++.h>
using namespace std;

struct TrieNode {
    TrieNode* children[26] = {nullptr};
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
            int position = c - 'a';
            if (!node->children[position]) node->children[position] = new TrieNode();
            node = node->children[position];
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
                int position = c - 'a';
                if (!node->children[position]) break;
                node = node->children[position];
                steps += node->count;
            }

            cout << steps << "\n";
        }
    }
}
