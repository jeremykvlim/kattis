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

    int n, k;
    cin >> n >> k;

    vector<double> p(n), d(n);
    for (int i = 0; i < n; i++) cin >> p[i] >> d[i];

    auto pref = p, suff = p;
    for (int i = 1; i < n; i++) pref[i] += ldexp(pref[i - 1], d[i - 1] - d[i]);
    for (int i = n - 2; ~i; i--) suff[i] += ldexp(suff[i + 1], d[i] - d[i + 1]);

    vector<double> dp(n + 1), base1(n), base2(n);
    vector<int> count1(n + 1), count2(n);
    auto construct = [&](double m) {
        dp[0] = count1[0] = 0;

        auto get1 = [&](int row, int col) {
            return base1[col] + pref[row] - (!col ? 0 : ldexp(pref[col - 1], d[col - 1] - d[row]));
        };

        auto cmp1 = [&](const auto &p1, const auto &p2) {
            return p1.first != p2.first ? p1.first < p2.first : count1[p1.second] < count1[p2.second];
        };

        auto get2 = [&](int row, int col) {
            return base2[col] + suff[col] - (row + 1 >= n ? 0 : ldexp(suff[row + 1], d[col] - d[row + 1]));
        };

        auto cmp2 = [&](const auto &p1, const auto &p2) {
            return p1.first != p2.first ? p1.first < p2.first : count2[p1.second] < count2[p2.second];
        };

        LARSCH<double> larsch1(n), larsch2(n);
        for (int i = 0; i < n; i++) {
            base1[i] = dp[i] - m;
            auto [value1, col1] = larsch1.query(get1, cmp1);

            base2[i] = value1 - p[i];
            count2[i] = count1[col1] + 1;
            auto [value2, col2] = larsch2.query(get2, cmp2);

            if (dp[i] < value2 || dp[i] == value2 && count1[i] < count2[col2]) {
                dp[i + 1] = value2;
                count1[i + 1] = count2[col2];
            } else {
                dp[i + 1] = dp[i];
                count1[i + 1] = count1[i];
            }
        }
        return count1[n];
    };

    double l = 0, r = 3 * *max_element(p.begin(), p.end()), m;
    while (l + 1e-6 < r && l + l * 1e-6 < r) {
        m = midpoint(l, r);

        if (construct(m) > k) l = m;
        else r = m;
    }
    cout << fixed << setprecision(6) << dp[n] + m * k;
}