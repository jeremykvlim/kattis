#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int r, c;
    cin >> r >> c;
    cin.ignore();
    r += 2;
    c += 2;

    int n = r * c;
    auto index = [&](int i, int j) {
        return i * c + j;
    };

    vector<char> grid(n);
    array<int, 3> villain_indices;
    int bell = -1;
    for (int i = 0; i < r; i++) {
        string row;
        getline(cin, row);
        for (int j = 0; j < c; j++) {
            char ch = row[j];
            if (ch == '+' || ch == '-' || ch == '|') ch = 'o';

            int v = index(i, j);
            if (ch == 'c') villain_indices[0] = v;
            else if (ch == 'l') villain_indices[1] = v;
            else if (ch == 'q') villain_indices[2] = v;
            else if (ch == '%') bell = v;
            grid[v] = ch;
        }
    }

    array<char, 4> dirs{'v','>', '^','<'};
    vector<int> dr{1, 0, -1, 0}, dc{0, 1, 0, -1};
    vector<vector<int>> next(4, vector<int>(n, 0));
    for (int k = 0; k < 4; k++)
        for (int i = (!k ? r - 2 : 1); i != (!k ? 0 : r - 1); i += (!k ? -1 : 1))
            for (int j = (k == 1 ? c - 2 : 1); j != (k == 1 ? 0 : c - 1); j += (k == 1 ? -1 : 1)) {
                int v = index(i, j), u = index(i + dr[k], j + dc[k]);

                if (grid[u] == '.') next[k][v] = next[k][u];
                else if (grid[u] == '#') next[k][v] = v;
                else next[k][v] = u;
            }

    array<char, 3> villains{'C', 'L', 'Q'};
    vector<vector<pair<int, char>>> prev(8, vector<pair<int, char>>(n, {-1, '?'}));
    queue<int> q;
    vector<int> submask(n, 0);
    for (int mask = 1; mask < 8; mask++) {
        int count = popcount((unsigned) mask);
        if (count == 1) {
            int v = villain_indices[countr_zero((unsigned) mask)];
            prev[mask][v] = {0, '!'};
            q.emplace(v);
        } else if (count == 2) {
            int i = countr_zero((unsigned) (~mask) & 7), v1 = mask & -mask, v2 = mask ^ v1;
            for (int v = 0; v < n; v++)
                if (grid[v] != 'o' && grid[v] != villains[i] && ~prev[v1][v].first && ~prev[v2][v].first) {
                    prev[mask][v] = {0, '!'};
                    q.emplace(v);
                }
        } else {
            for (int v = 0; v < n; v++)
                if (grid[v] != 'o') {
                    for (int m = mask; m; m &= m - 1) {
                        int v1 = m & -m, v2 = mask ^ v1;

                        if (~prev[v1][v].first && ~prev[v2][v].first) {
                            prev[mask][v] = {0, '!'};
                            q.emplace(v);
                            submask[v] = v1;
                            break;
                        }
                    }
                }
        }

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            auto valid = [&](int mask, int v) -> bool {
                char ch = grid[v];
                if (ch == ' ' || ch == '.' || ch == '%' || ch == 'c' || ch == 'l' || ch == 'q') return true;
                if (ch == 'C') return mask & 1;
                if (ch == 'L') return mask & 2;
                if (ch == 'Q') return mask & 4;
                return false;
            };

            if (!valid(mask, v)) continue;

            for (int k = 0; k < 4; k++) {
                int u = next[k][v];
                if (!~prev[mask][u].first) {
                    prev[mask][u] = {v, dirs[k]};
                    q.emplace(u);
                }
            }
        }
    }

    if (!~prev[7][bell].first) {
        cout << "IMPOSSIBLE";
        exit(0);
    }

    vector<pair<char, char>> moves;
    auto reconstruct = [&](auto &&self, int mask, int v) -> void {
        while (prev[mask][v].second != '!') {
            auto [u, dir] = prev[mask][v];
            for (int i = 0; i < 3; i++)
                if ((mask >> i) & 1)
                    if (grid[v] != villains[i]) moves.emplace_back(tolower(villains[i]), dir);

            for (int i = 0; i < 3; i++)
                if ((mask >> i) & 1)
                    if (grid[v] == villains[i]) moves.emplace_back(tolower(villains[i]), dir);
            v = u;
        }

        int count = popcount((unsigned) mask);
        if (count == 1) return;

        int v1 = count == 2 ? mask & -mask : submask[v], v2 = mask ^ v1;
        if (grid[v] == villains[countr_zero((unsigned) v1)]) swap(v1, v2);
        self(self, v1, v);
        self(self, v2, v);
    };
    reconstruct(reconstruct, 7, bell);
    reverse(moves.begin(), moves.end());
    cout << moves.size() << "\n";
    for (auto [v, d] : moves) cout << v << " " << d << "\n";
}
