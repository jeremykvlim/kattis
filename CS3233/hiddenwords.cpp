#include <bits/stdc++.h>
using namespace std;

struct TrieNode {
    TrieNode* children[26] = {nullptr};
    int count = 0;
};

void dfs(vector<vector<bool>> &visited, vector<string> &grid, int i, int j, TrieNode* node, int &words) {
    visited[i][j] = true;
    words += node->count;
    node->count = 0;

    vector<int> dr = {1, -1, 0, 0}, dc = {0, 0, 1, -1};
    for (int k = 0; k < 4; k++) {
        int r = i + dr[k], c = j + dc[k];
        if (r < 0 || r >= grid.size() || c < 0 || c >= grid[0].size() || visited[r][c]) continue;
        int position = grid[r][c] - 'A';
        if (node->children[position]) dfs(visited, grid, r, c, node->children[position], words);
    }
    visited[i][j] = false;
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
            int position = c - 'A';
            if (!node->children[position]) node->children[position] = new TrieNode();
            node = node->children[position];
        }
        node->count++;
    }

    int words = 0;
    vector<vector<bool>> visited(h, vector<bool>(w, false));
    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++) {
            int position = grid[i][j] - 'A';
            if (root->children[position]) dfs(visited, grid, i, j, root->children[position], words);
        }

    cout << words;
}
