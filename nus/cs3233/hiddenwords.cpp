#include <bits/stdc++.h>
using namespace std;

struct TrieNode {
    vector<int> next;
    int count = 0;

    TrieNode() {
        next.resize(26, -1);
    }
};

int dfs(vector<vector<bool>> &visited, vector<string> &grid, int i, int j, vector<TrieNode> &trie, int node) {
    visited[i][j] = true;
    int words = trie[node].count;
    trie[node].count = 0;

    vector<int> dr{1, -1, 0, 0}, dc{0, 0, 1, -1};
    for (int k = 0; k < 4; k++) {
        int r = i + dr[k], c = j + dc[k];
        if (r < 0 || r >= grid.size() || c < 0 || c >= grid[0].size() || visited[r][c]) continue;

        int pos = grid[r][c] - 'A';
        if (trie[node].next[pos] != -1) words += dfs(visited, grid, r, c, trie, trie[node].next[pos]);
    }
    visited[i][j] = false;

    return words;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int h, w, n;
    cin >> h >> w;

    vector<string> grid(h);
    for (auto &row : grid) cin >> row;
    cin >> n;

    vector<TrieNode> trie;
    trie.emplace_back();
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        int node = 0;
        for (char c : s) {
            int pos = c - 'A';

            if (trie[node].next[pos] == -1) {
                trie[node].next[pos] = trie.size();
                trie.emplace_back();
            }
            node = trie[node].next[pos];
        }
        trie[node].count++;
    }

    int words = 0;
    vector<vector<bool>> visited(h, vector<bool>(w, false));
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++) {
            int pos = grid[i][j] - 'A';
            if (trie[0].next[pos] != -1) words += dfs(visited, grid, i, j, trie, trie[0].next[pos]);
        }

    cout << words;
}
