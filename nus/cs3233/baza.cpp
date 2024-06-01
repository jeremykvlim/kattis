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

    unordered_map<string, int> database;
    vector<TrieNode> trie(1);
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        int steps = i, node = 0;
        for (char c : s) {
            int pos = c - 'a';

            if (trie[node].next[pos] == -1) {
                trie[node].next[pos] = trie.size();
                trie.emplace_back();
            }
            node = trie[node].next[pos];
            steps += ++trie[node].count;
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
            int steps = n, node = 0;
            for (char c : s) {
                int pos = c - 'a';
                if (trie[node].next[pos] == -1) break;
                node = trie[node].next[pos];
                steps += trie[node].count;
            }

            cout << steps << "\n";
        }
    }
}
