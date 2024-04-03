#include <bits/stdc++.h>
using namespace std;

struct TrieNode {
    vector<int> next;

    TrieNode() {
        next.resize(26, -1);
    }
};

void dfs(int start, int curr, int end, vector<TrieNode> &trie, vector<bool> &visited, string &s) {
    if (curr != end) visited[curr] = true;

    for (int i = start; i < s.size(); i++) {
        int pos = s[i] - 'a';
        if (trie[curr].next[pos] != -1) dfs(i + 1, trie[curr].next[pos], end, trie, visited, s);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<TrieNode> trie;
    trie.emplace_back();
    vector<string> sorted, S;
    vector<int> end;
    string s;
    while (cin >> s) {
        S.emplace_back(s);
        sort(s.begin(), s.end());
        sorted.emplace_back(s);

        int node = 0;
        for (char c : s) {
            int pos = c - 'a';

            if (trie[node].next[pos] == -1) {
                trie[node].next[pos] = trie.size();
                trie.emplace_back();
            }
            node = trie[node].next[pos];
        }
        end.emplace_back(node);
    }

    vector<bool> visited(*max_element(end.begin(), end.end()) + 1, false);
    for (int i = 0; i < end.size(); i++) dfs(0, 0, end[i], trie, visited, sorted[i]);

    vector<string> dominant;
    for (int i = 0; i < end.size(); i++)
        if (!visited[end[i]]) dominant.emplace_back(S[i]);
    sort(dominant.begin(), dominant.end());
    for (auto &d : dominant) cout << d << "\n";
}
