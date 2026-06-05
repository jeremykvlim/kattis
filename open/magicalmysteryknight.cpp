#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> dr{2, 2, 1, -1, -2, -2, -1, 1}, dc{1, -1, -2, -2, -1, 1, 2, 2};
    vector<unsigned long long> knight(64);
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++) {
            int sq = r * 8 + c;
            for (int k = 0; k < 8; k++) {
                int x = r + dr[k], y = c + dc[k];
                if (0 <= x && x < 8 && 0 <= y && y < 8) knight[sq] |= 1ULL << (x * 8 + y);
            }
        }

    auto removed_mask = 0ULL;
    vector<int> pos(65, -1), row_sum(8), col_sum(8), row_removed(8), col_removed(8);
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8; c++) {
            int v;
            cin >> v;

            int sq = r * 8 + c;
            if (v == -1) {
                removed_mask |= 1ULL << sq;
                row_removed[r]++;
                col_removed[c]++;
            } else {
                pos[v] = sq;
                row_sum[r] += v;
                col_sum[c] += v;
            }
        }

    auto dfs = [&](auto &&self) -> bool {
        vector<int> removed_labels;
        for (int v = 1; v <= 64; v++)
            if (pos[v] == -1) removed_labels.emplace_back(v);

        for (int i = 0; i < 8; i++)
            for (int dir = 0; dir < 2; dir++) {
                int remaining = !dir ? 260 - row_sum[i] : 260 - col_sum[i];
                if (remaining < 0) return false;

                int pref = 0, suff = 0;
                for (int j = 0; j < (!dir ? row_removed[i] : col_removed[i]); j++) {
                    pref += removed_labels[j];
                    suff += removed_labels[removed_labels.size() - 1 - j];
                }
                if (remaining < pref || remaining > suff) return false;
            }

        for (int v = 1; v < 64; v++) {
            if (pos[v] == -1 || pos[v + 1] == -1) continue;
            if (!((knight[pos[v]] >> pos[v + 1]) & 1)) return false;
        }
        if (!removed_mask) return true;

        int u = -1, w = -1;
        for (int v = 2; v <= 64; v++) {
            if (pos[v] != -1 || pos[v - 1] == -1) continue;
            for (u = w = v; w < 64 && pos[w + 1] == -1; w++);
            break;
        }
        if (u == -1 && w == -1) return false;

        if (popcount(removed_mask) == w - u + 1) {
            bool bounded = w < 64 && pos[w + 1] != -1;

            auto nodes = removed_mask | (1ULL << pos[u - 1]);
            if (bounded) nodes |= 1ULL << pos[w + 1];

            for (auto mask = removed_mask; mask; mask &= mask - 1) {
                int degree = popcount(knight[countr_zero(mask)] & nodes);
                if (bounded && degree < 2 || !bounded && !degree) return false;
            }
        }

        for (auto mask = knight[pos[u - 1]] & removed_mask; mask; mask &= mask - 1) {
            int sq = countr_zero(mask), r = sq / 8, c = sq % 8;
            if ((removed_mask >> sq) & 1 && row_sum[r] + u <= 260 && col_sum[c] + u <= 260) {
                removed_mask ^= 1ULL << sq;
                pos[u] = sq;
                row_sum[r] += u;
                col_sum[c] += u;
                row_removed[r]--;
                col_removed[c]--;
                if (self(self)) return true;
                col_removed[c]++;
                row_removed[r]++;
                col_sum[c] -= u;
                row_sum[r] -= u;
                pos[u] = -1;
                removed_mask |= 1ULL << sq;
            }
        }
        return false;
    };
    dfs(dfs);

    vector<vector<int>> board(8, vector<int>(8));
    for (int v = 1; v <= 64; v++) board[pos[v] / 8][pos[v] % 8] = v;

    for (auto &row : board) {
        for (int v : row) cout << v << " ";
        cout << "\n";
    }
}
