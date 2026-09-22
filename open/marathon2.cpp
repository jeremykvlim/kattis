#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct LARSCH {
    int n, lg, i;
    vector<pair<T, int>> dp, cols;
    vector<int> offset, size, pos;

    LARSCH(int n) : n(n), lg(__lg(n)), i(0), dp(lg), offset(lg + 1), size(lg), pos(lg) {
        for (int p2 = 0; p2 < lg; p2++) offset[p2 + 1] = offset[p2] + (n >> (p2 + 1));
        cols.resize(offset[lg]);
    }

    pair<T, int> query(auto &&get, auto &&cmp) {
        int p2 = 0;
        pair<T, int> optimum;
        for (;;) {
            int r = i >> p2, limit = (r + 1) >> 1, col = !p2 ? i : cols[offset[p2 - 1] + r].second;
            if (p2 == lg) {
                int row = ((r + 1) << p2) - 1;
                if (!p2) optimum = {get(row, col), col};
                else optimum = cols[offset[p2 - 1] + r];
                p2--;
                break;
            }

            if (r & 1) {
                int row = (limit << (p2 + 1)) - 1;
                pair<T, int> p;
                if (!p2) p = {get(row, col), col};
                else p = cols[offset[p2 - 1] + r];
                if (pos[p2] == r || cmp(dp[p2], p)) {
                    pos[p2] = r;
                    dp[p2] = p;
                }
            }

            if (pos[p2] == r) {
                size[p2] = limit;
                if (p2 + 1 < lg) pos[p2 + 1] = limit;
            }

            int temp = size[p2];
            for (; size[p2] > limit; size[p2]--) {
                int row = (size[p2] << (p2 + 1)) - 1;
                pair<T, int> p{get(row, col), col};
                if (!cmp(cols[offset[p2] + size[p2] - 1], p)) break;
                cols[offset[p2] + size[p2] - 1] = p;
            }

            if (temp != size[p2]) size[p2]++;
            else {
                int row = ((size[p2] + 1) << (p2 + 1)) - 1;
                if (row < n) cols[offset[p2] + size[p2]++] = {get(row, col), col};
            }

            if (r & 1) {
                optimum = dp[p2];
                p2--;
                break;
            }

            if ((limit + 1) << (p2 + 1) > n) {
                optimum.second = col;
                break;
            }

            p2++;
        }

        for (; ~p2; p2--) {
            dp[p2] = optimum;
            int r = i >> p2, row = ((r + 1) << p2) - 1, stop = dp[p2].second, col = !p2 ? pos[p2] : cols[offset[p2 - 1] + pos[p2]].second;
            auto next = [&]() -> pair<T, int> {
                if (!p2 || pos[p2] != r) return {get(row, col), col};
                return cols[offset[p2 - 1] + r];
            };
            optimum = next();
            if (col == stop) continue;
            for (pos[p2]++;; pos[p2]++) {
                col = !p2 ? pos[p2] : cols[offset[p2 - 1] + pos[p2]].second;
                auto p = next();
                if (cmp(optimum, p)) optimum = p;
                if (col == stop) break;
            }
        }
        i++;
        return optimum;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, x, y, h, s;
    cin >> n >> x >> y >> h >> s;

    vector<int> positions(n);
    for (int &position : positions) cin >> position;
    positions.emplace_back(0);
    positions.emplace_back(42195);
    sort(positions.begin(), positions.end());
    positions.erase(unique(positions.begin(), positions.end()), positions.end());

    int p = positions.size(), m = lcm(h, s);
    h = m / h;
    s = m / s;
    vector<int> dp(p, 0);
    auto get = [&](int row, int col) {
        int j = row + 1;
        int d = positions[j] - positions[col];
        if (d <= x) return dp[col] + h * d;
        return dp[col] + h * x + s * (d - x);
    };

    auto cmp = [&](const auto &p1, const auto &p2) {
        return p1.first > p2.first;
    };

    LARSCH<int> larsch(p - 1);
    for (int j = 1; j < p; j++) {
        dp[j] = larsch.query(get, cmp).first;
        if (j + 1 < p) dp[j] += y * m;
    }

    int seconds = dp[p - 1] / m;
    cout << setw(2) << setfill('0') << seconds / 3600 << ":" << setw(2) << (seconds % 3600) / 60 << ":" << setw(2) << seconds % 60;
}
