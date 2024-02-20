#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;

struct Die {
    int up, down, left, right, front, back, height, width;

    bool operator==(const Die &d) const {
        return tie(up, down, left, right, front, back, height, width) ==
               tie(d.up, d.down, d.left, d.right, d.front, d.back, d.height, d.width);
    }

    uint64_t encode() const {
        uint64_t encoded = 0;
        encoded = (encoded << 8) | up;
        encoded = (encoded << 8) | down;
        encoded = (encoded << 8) | left;
        encoded = (encoded << 8) | right;
        encoded = (encoded << 8) | front;
        encoded = (encoded << 8) | back;
        encoded = (encoded << 16) | (height << 8) | width;
        return encoded;
    }
};

struct Hash {
     static uint64_t h(uint64_t key) {
        uint64_t hash = key + 0x9e3779b97f4a7c15;
        hash = (hash ^ (hash >> 30)) * 0xbf58476d1ce4e5b9;
        hash = (hash ^ (hash >> 27)) * 0x94d049bb133111eb;
        hash = hash ^ (hash >> 31);
        return hash;
    }

    size_t operator()(const Die &d) const {
        static const uint64_t SEED = chrono::steady_clock::now().time_since_epoch().count();
        return h(d.encode() + SEED);
    }
};

void rotate(Die &s, char direction) {
    switch (direction) {
        case 'N':
            swap(s.down, s.back), swap(s.back, s.up), swap(s.up, s.front);
            break;
        case 'S':
            swap(s.front, s.up), swap(s.up, s.back), swap(s.back, s.down);
            break;
        case 'E':
            swap(s.down, s.right), swap(s.right, s.up), swap(s.up, s.left);
            break;
        case 'W':
            swap(s.left, s.up), swap(s.up, s.right), swap(s.right, s.down);
            break;
    }
}

void dfs(Die curr, vector<vector<int>> &board, gp_hash_table<Die, null_type, Hash> &visited,
         vector<pair<pair<int, int>, char>> &directions) {

    if (curr.height + 1 == board.size() && curr.width + 1 == board[0].size()) {
        cout << "YES";
        exit(0);
    }

    for (auto d : directions) {
        Die next = curr;
        next.height += d.first.first;
        next.width += d.first.second;
        if (next.height < 0 || next.width < 0 || next.height >= board.size() || next.width >= board[0].size()) continue;
        rotate(next, d.second);

        if (next.down == -1) next.down = board[next.height][next.width];
        if (next.down != board[next.height][next.width]) continue;

        if (visited.find(next) == visited.end()) {
            visited.insert(next);
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

    vector<pair<pair<int, int>, char>> directions = {{{-1, 0}, 'N'},
                                                     {{1,  0}, 'S'},
                                                     {{0,  -1}, 'W'},
                                                     {{0,  1}, 'E'}};

    gp_hash_table<Die, null_type, Hash> visited;
    Die start = {-1, board[0][0], -1, -1, -1, -1, 0, 0};
    visited.insert(start);
    dfs(start, board, visited, directions);

    cout << "NO";
}
