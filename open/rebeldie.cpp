#include <bits/stdc++.h>
using namespace std;

struct Hash {
    static uint64_t encode(array<int, 8> a) {
        auto encoded = 0ULL;
        for (int e : a) encoded = (encoded << 8) | e;
        return encoded;
    }

    static uint64_t h(uint64_t key) {
        auto hash = key + 0x9e3779b97f4a7c15;
        hash = (hash ^ (hash >> 30)) * 0xbf58476d1ce4e5b9;
        hash = (hash ^ (hash >> 27)) * 0x94d049bb133111eb;
        hash = hash ^ (hash >> 31);
        return hash;
    }

    size_t operator()(array<int, 8> a) const {
        static uint64_t SEED = chrono::steady_clock::now().time_since_epoch().count();
        return h(encode(a) + SEED);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, n;
    cin >> m >> n;

    vector<vector<int>> board(m, vector<int>(n));
    for (auto &row : board)
        for (int &square : row) cin >> square;

    vector<pair<pair<int, int>, char>> directions{{{-1, 0}, 'N'},
                                                  {{1,  0}, 'S'},
                                                  {{0,  1}, 'E'},
                                                  {{0,  -1}, 'W'}};

    unordered_set<array<int, 8>, Hash> visited;
    array<int, 8> start{-1, board[0][0], -1, -1, -1, -1, 0, 0};
    visited.emplace(start);

    auto dfs = [&](auto &&self, auto curr) -> void {
        if (curr[6] + 1 == board.size() && curr[7] + 1 == board[0].size()) {
            cout << "YES";
            exit(0);
        }

        for (auto [d, c] : directions) {
            auto next = curr;
            next[6] += d.first;
            next[7] += d.second;
            if (!(0 <= next[6] && next[6] < board.size() && 0 <= next[7] && next[7] < board[0].size())) continue;

            auto rotate = [&]() {
                switch (c) {
                    case 'N':
                        swap(next[1], next[5]);
                        swap(next[5], next[0]);
                        swap(next[0], next[4]);
                        break;
                    case 'S':
                        swap(next[4], next[0]);
                        swap(next[0], next[5]);
                        swap(next[5], next[1]);
                        break;
                    case 'E':
                        swap(next[1], next[3]);
                        swap(next[3], next[0]);
                        swap(next[0], next[2]);
                        break;
                    case 'W':
                        swap(next[2], next[0]);
                        swap(next[0], next[3]);
                        swap(next[3], next[1]);
                        break;
                }
            };
            rotate();

            if (next[1] == -1) next[1] = board[next[6]][next[7]];
            if (next[1] != board[next[6]][next[7]]) continue;

            if (!visited.count(next)) {
                visited.emplace(next);
                self(self, next);
            }
        }
    };
    dfs(dfs, start);

    cout << "NO";
}
