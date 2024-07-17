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

void rotate(array<int, 8> &state, char dir) {
    switch (dir) {
        case 'N':
            swap(state[1], state[5]);
            swap(state[5], state[0]);
            swap(state[0], state[4]);
            break;
        case 'S':
            swap(state[4], state[0]);
            swap(state[0], state[5]);
            swap(state[5], state[1]);
            break;
        case 'E':
            swap(state[1], state[3]);
            swap(state[3], state[0]);
            swap(state[0], state[2]);
            break;
        case 'W':
            swap(state[2], state[0]);
            swap(state[0], state[3]);
            swap(state[3], state[1]);
            break;
    }
}

void dfs(array<int, 8> curr, vector<vector<int>> &board, unordered_set<array<int, 8>, Hash> &visited, vector<pair<pair<int, int>, char>> &directions) {
    if (curr[6] + 1 == board.size() && curr[7] + 1 == board[0].size()) {
        cout << "YES";
        exit(0);
    }

    for (auto [d, c] : directions) {
        auto next = curr;
        next[6] += d.first;
        next[7] += d.second;
        if (!(0 <= next[6] && next[6] < board.size() && 0 <= next[7] && next[7] < board[0].size())) continue;

        rotate(next, c);

        if (next[1] == -1) next[1] = board[next[6]][next[7]];
        if (next[1] != board[next[6]][next[7]]) continue;

        if (!visited.count(next)) {
            visited.emplace(next);
            dfs(next, board, visited, directions);
        }
    }
}

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
    dfs(start, board, visited, directions);

    cout << "NO";
}
