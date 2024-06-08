#include <bits/stdc++.h>
using namespace std;

struct Trie {
    enum ascii {
        LOWER = 97,
        UPPER = 65,
        NUM = 48,
        NA = 0
    };

    struct TrieNode {
        vector<int> next;
        int count = 0;

        TrieNode(int range = 26) : next(range, -1) {}
    };

    vector<TrieNode> T;
    ascii a;

    Trie(int n = 1, ascii alpha = LOWER, int range = 26) : T(n, TrieNode(range)), a(alpha) {}

    void add(string &s) {
        int node = 0;
        for (char c : s) {
            int pos = c - a;

            if (T[node].next[pos] == -1) {
                T[node].next[pos] = T.size();
                T.emplace_back();
            }
            node = T[node].next[pos];
        }
        T[node].count++;
    }

    auto & operator[](int i) {
        return T[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int h, w;
    cin >> h >> w;

    vector<string> grid(h);
    for (auto &row : grid) cin >> row;

    int n;
    cin >> n;

    Trie trie(1, Trie::UPPER);
    while (n--) {
        string s;
        cin >> s;

        trie.add(s);
    }

    int words = 0;
    vector<vector<bool>> visited(h, vector<bool>(w, false));
    auto dfs = [&](auto &&self, int i, int j, int node) -> int {
        visited[i][j] = true;
        int words = trie[node].count;
        trie[node].count = 0;

        vector<int> dr{1, -1, 0, 0}, dc{0, 0, 1, -1};
        for (int k = 0; k < 4; k++) {
            int r = i + dr[k], c = j + dc[k];
            if (0 <= r && r < grid.size() && 0 <= c && c < grid[0].size() && !visited[r][c]) {
                int pos = grid[r][c] - 'A';
                if (trie[node].next[pos] != -1) words += self(self, r, c, trie[node].next[pos]);
            }
        }
        visited[i][j] = false;

        return words;
    };

    for (int i = 0; i < h; i++)
        for (int j = 0; j < w; j++) {
            int pos = grid[i][j] - 'A';
            if (trie[0].next[pos] != -1) words += dfs(dfs, i, j, trie[0].next[pos]);
        }

    cout << words;
}
