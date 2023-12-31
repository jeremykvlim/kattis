#include <bits/stdc++.h>
using namespace std;

struct TrieNode {
    TrieNode* children[26] = {nullptr};
};

void dfs(int start, TrieNode* curr, TrieNode* self, unordered_map<TrieNode*, bool> &visited, string &s) {
    if (curr != self) visited[curr] = true;
    for (int i = start; i < s.size(); i++) {
        int pos = s[i] - 'a';
        if (curr->children[pos]) dfs(i + 1, curr->children[pos], self, visited, s);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto *root = new TrieNode();
    vector<string> sorted, S;
    vector<TrieNode*> end;
    unordered_map<TrieNode*, bool> visited;
    string s;
    while (cin >> s) {
        S.emplace_back(s);
        sort(s.begin(), s.end());
        sorted.emplace_back(s);

        auto *node = root;
        for (char c : s) {
            int pos = c - 'a';
            if (!node->children[pos]) node->children[pos] = new TrieNode();
            node = node->children[pos];
        }
        end.emplace_back(node);
    }
    for (int i = 0; i < end.size(); i++) dfs(0, root, end[i], visited, sorted[i]);

    vector<string> dominant;
    for (int i = 0; i < end.size(); i++)
        if (!visited[end[i]]) dominant.emplace_back(S[i]);
    sort(dominant.begin(), dominant.end());
    for (auto &d : dominant) cout << d << "\n";
}
