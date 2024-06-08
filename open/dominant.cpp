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
    vector<int> ends;

    Trie(int n = 1) : T(n) {}

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
        ends.emplace_back(node);
    }

    auto size() {
        return T.size();
    }

    auto & operator[](int i) {
        return T[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Trie trie;
    vector<string> sorted, S;
    string s;
    while (cin >> s) {
        S.emplace_back(s);
        sort(s.begin(), s.end());
        sorted.emplace_back(s);

        trie.add(s);
    }

    vector<bool> visited(trie.size(), false);
    auto dfs = [&](auto &&self, int i, int node, int end, string s) -> void {
        if (node != end) visited[node] = true;

        for (; i < s.size(); i++) {
            int pos = s[i] - 'a';
            if (trie[node].next[pos] != -1) self(self, i + 1, trie[node].next[pos], end, s);
        }
    };
    for (int i = 0; i < trie.ends.size(); i++) dfs(dfs, 0, 0, trie.ends[i], sorted[i]);

    vector<string> dominant;
    for (int i = 0; i < trie.ends.size(); i++)
        if (!visited[trie.ends[i]]) dominant.emplace_back(S[i]);
    sort(dominant.begin(), dominant.end());
    
    for (auto d : dominant) cout << d << "\n";
}
