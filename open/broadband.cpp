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

    int m, n, k, s;
    cin >> m >> n >> k >> s;
    n += m;

    vector<int> x(n);
    for (int &xi : x) {
        double X;
        cin >> X;

        xi = round(X * 1e4);
    }
    sort(x.begin(), x.end());

    vector<int> pref_w{0};
    vector<long long> pref_wx{0}, pref_wxx{0};
    for (int i = 0, j = 1; i < n; i = j++) {
        for (; j < n && x[i] == x[j]; j++);

        int w = j - i;
        pref_w.emplace_back(pref_w.back() + w);
        pref_wx.emplace_back(pref_wx.back() + (long long) w * x[i]);
        pref_wxx.emplace_back(pref_wxx.back() + (long long) w * x[i] * x[i]);
    }

    m = pref_w.size() - 1;
    auto base = (double) n * s * s / 4;
    if (m <= k) {
        cout << fixed << setprecision(5) << base;
        exit(0);
    }

    auto cost = [&](int l, int r) {
        int w = pref_w[r] - pref_w[l];
        auto wx = pref_wx[r] - pref_wx[l];
        return (double) ((__int128) w * (pref_wxx[r] - pref_wxx[l]) - (__int128) wx * wx) / (w * 1e8);
    };

    if (k == 1) {
        cout << fixed << setprecision(5) << base + cost(0, m);
        exit(0);
    }

    vector<pair<double, int>> dp(m + 1);
    double l = -1, r = cost(0, m), mid;
    while (l + 1e-6 < r && l + l * 1e-6 < r) {
        mid = l + (r - l) / 2;

        auto get = [&](int row, int col) -> pair<double, int> {
            return {dp[col].first + cost(col, row + 1) + mid, dp[col].second + 1};
        };

        auto cmp = [&](const auto &p1, const auto &p2) {
            return p1 > p2;
        };

        LARSCH<pair<double, int>> larsch(m);
        for (int i = 1; i <= m; i++) dp[i] = larsch.query(get, cmp).first;

        if (dp[m].second > k) l = mid;
        else r = mid;
    }
    cout << fixed << setprecision(5) << base + dp[m].first - mid * k;
}