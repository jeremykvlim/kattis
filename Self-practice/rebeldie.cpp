#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;

struct Die {
    int up, down, left, right, front, back, height, width;

    bool operator==(const Die &s) const {
        return tie(up, down, left, right, front, back, height, width) == tie(s.up, s.down, s.left, s.right, s.front, s.back, s.height, s.width);
    }

    uint64_t key() const { return (((((((((((uint64_t(up) << 8) | down) << 8) | left) << 8) | right) << 8) | front) << 8) | back) << 16) | (height << 8) | width; }
};

struct Hash {
    static uint64_t h(uint64_t key) {
        key += 0x9e3779b97f4a7c15;
        key = (key ^ (key >> 30)) * 0xbf58476d1ce4e5b9;
        key = (key ^ (key >> 27)) * 0x94d049bb133111eb;
        return key ^ (key >> 31);
    }

    size_t operator()(const Die &d) const {
        static const uint64_t SEED = chrono::steady_clock::now().time_since_epoch().count();
        return h(d.key() + SEED);
    }
};

void rotate(Die &s, char direction) {
    switch (direction) {
        case 'N': swap(s.down, s.back), swap(s.back, s.up), swap(s.up, s.front); break;
        case 'S': swap(s.front, s.up), swap(s.up, s.back), swap(s.back, s.down); break;
        case 'E': swap(s.down, s.right), swap(s.right, s.up), swap(s.up, s.left); break;
        case 'W': swap(s.left, s.up), swap(s.up, s.right), swap(s.right, s.down); break;
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

    vector<pair<pair<int, int>, char>> directions = {{{-1, 0},  'N'},
                                                     {{1,  0},  'S'},
                                                     {{0,  -1}, 'W'},
                                                     {{0,  1},  'E'}};


    gp_hash_table<Die, null_type, Hash> visited;
    stack<Die> dfs;
    dfs.push({-1, board[0][0], -1, -1, -1, -1, 0, 0});

    while (!dfs.empty()) {
        Die curr = dfs.top();
        dfs.pop();

        if (curr.height == m - 1 && curr.width == n - 1) {
            cout << "YES";
            exit(0);
        }

        for (auto d : directions) {
            Die next = curr;
            next.height += d.first.first;
            next.width += d.first.second;
            if (next.height < 0 || next.width < 0 || next.height >= m || next.width >= n) continue;
            rotate(next, d.second);

            if (next.down == -1) next.down = board[next.height][next.width];
            if (next.down != board[next.height][next.width]) continue;

            if (visited.find(next) == visited.end()) {
                visited.insert(next);
                dfs.push(next);
            }
        }
    }
  
    cout << "NO";
}
