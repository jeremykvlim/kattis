#include <bits/stdc++.h>
using namespace std;

struct TrieNode {
    TrieNode *children[26] = {nullptr};
    int count = 0;
};

int dfs(vector<vector<bool>> &visited, vector<string> &grid, int i, int j, TrieNode *node) {
    visited[i][j] = true;
    int words = node->count;
    node->count = 0;

    vector<int> dr{1, -1, 0, 0}, dc{0, 0, 1, -1};
    for (int k = 0; k < 4; k++) {
        int r = i + dr[k], c = j + dc[k];
        if (r < 0 || r >= grid.size() || c < 0 || c >= grid[0].size() || visited[r][c]) continue;
        int pos = grid[r][c] - 'A';
        if (node->children[pos]) words += dfs(visited, grid, r, c, node->children[pos]);
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

    auto *root = new TrieNode();
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        auto *node = root;
        for (char c : s) {
            int pos = c - 'A';
            if (!node->children[pos]) node->children[pos] = new TrieNode();
            node = node->children[pos];
        }
        node->count++;
    }

    int words = 0;
    vector<vector<bool>> visited(h, vector<bool>(w, false));
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++) {
            int pos = grid[i][j] - 'A';
            if (root->children[pos]) words += dfs(visited, grid, i, j, root->children[pos]);
        }

    cout << words;
}
