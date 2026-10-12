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

    int n, m;
    cin >> n >> m;

    vector<int> pref_k{0};
    vector<long long> pref_ki{0}, pref_kii{0};
    for (int i = 1; i <= n; i++) {
        long long k;
        cin >> k;

        if (!k) continue;
        pref_k.emplace_back(pref_k.back() + k);
        pref_ki.emplace_back(pref_ki.back() + k * i);
        pref_kii.emplace_back(pref_kii.back() + k * i * i);
    }
    
    n = pref_k.size() - 1;
    if (n <= m) {
        cout << 0;
        exit(0);
    }

    auto cost = [&](int l, int r) {
        int k = pref_k[r] - pref_k[l];
        auto ki = pref_ki[r] - pref_ki[l];
        return (double) ((__int128) k * (pref_kii[r] - pref_kii[l]) - (__int128) ki * ki) / k;
    };

    vector<pair<double, int>> dp(n + 1);
    double l = 0, r = cost(0, n), mid;
    while (l + 1e-6 < r && l + l * 1e-6 < r) {
        mid = l + (r - l) / 2;

        auto get = [&](int row, int col) -> pair<double, int> {
            return {dp[col].first + cost(col, row + 1) + mid, dp[col].second - 1};
        };

        auto cmp = [&](const auto &p1, const auto &p2) {
            return p1 > p2;
        };

        LARSCH<pair<double, int>> larsch(n);
        for (int i = 1; i <= n; i++) dp[i] = larsch.query(get, cmp).first;

        if (-dp[n].second > m) l = mid;
        else r = mid;
    }
    cout << fixed << setprecision(4) << dp[n].first - mid * m;
}