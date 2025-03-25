#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<string> board(n);
    for (auto &row : board) cin >> row;

    int count = 0;
    for (int r = 0; r < n; r++)
        for (int c = 0; c < n; c++)
            if (board[r][c] == 'B') {
                board[r][c] = '.';
                auto check = [&]() {
                    auto valid = [&](int i, int j) {
                        return 0 <= i && i < n && 0 <= j && j < n && (i & 1) == (r & 1) && (j & 1) == (c & 1) && (r + c) % 4 == (i + j) % 4 && board[i][j] != 'B' && board[i][j] != 'W';
                    };

                    vector<pair<int, int>> edges;
                    for (int i = 0; i < n; i++)
                        for (int j = 0; j < n; j++)
                            if (board[i][j] == 'W') {
                                if (valid(i - 1, j - 1) && valid(i + 1, j + 1)) edges.emplace_back((i - 1) * n + j - 1, (i + 1) * n + j + 1);
                                else if (valid(i - 1, j + 1) && valid(i + 1, j - 1)) edges.emplace_back((i - 1) * n + j + 1, (i + 1) * n + j - 1);
                                else return;
                            }

                    vector<int> degree(n * n, 0);
                    DisjointSets dsu(n * n);
                    degree[r * n + c]++;
                    for (auto [u, v] : edges) {
                        degree[u]++;
                        degree[v]++;
                        dsu.unite(u, v);
                    }

                    for (auto [u, v] : edges)
                        if (dsu.find(u) != dsu.find(r * n + c)) return;

                    int odd = 0;
                    for (int d : degree)
                        if (d & 1) odd++;

                    if (odd == 1) count++;
                };
                check();
                board[r][c] = 'B';
            }

    cout << count;
}
