#include <bits/stdc++.h>
using namespace std;

template <typename T>
vector<pair<T, int>> smawk(int n, int m, auto &&get, auto &&cmp) {
    int lg = __lg(n);
    vector<pair<T, int>> dp(n);
    vector<int> cols(n), offset(lg + 1, 0);
    for (int b = 0; b < lg; b++) {
        int size = 0;
        auto push = [&](int col, int limit = 0) {
            int temp = size;
            for (; size > limit; size--) {
                int row = (size << (b + 1)) - 1;
                pair<T, int> p{get(row, col), col};
                if (!cmp(dp[row], p)) break;
                dp[row] = p;
            }
            if (size == n >> (b + 1)) return;

            if (size == temp) {
                int row = ((size + 1) << (b + 1)) - 1;
                dp[row] = {get(row, col), col};
            }
            cols[offset[b] + size++] = col;
        };

        if (!b)
            for (int col = 0; col < m; col++) push(col);
        else
            for (int i = offset[b - 1]; i < offset[b]; i++) push(cols[i], (i - offset[b - 1]) >> 1);
        offset[b + 1] = offset[b] + size;
    }

    for (int b = lg; b; b--)
        for (int row = (1 << b) - 1, i = offset[b - 1]; row < n; row += 2 << b) {
            int stop = row + (1 << b) < n ? dp[row + (1 << b)].second : -1, col = cols[i];
            dp[row] = {get(row, col), col};
            if (col == stop) continue;
            for (i++; i < offset[b]; i++) {
                col = cols[i];
                pair<T, int> p{get(row, col), col};
                if (cmp(dp[row], p)) dp[row] = p;
                if (col == stop) break;
            }
        }

    for (int row = 0, col = 0; row < n; row += 2) {
        int stop = row + 1 < n ? dp[row + 1].second : -1;
        dp[row] = {get(row, col), col};
        if (col == stop) continue;
        for (col++; col < m; col++) {
            pair<T, int> p{get(row, col), col};
            if (cmp(dp[row], p)) dp[row] = p;
            if (col == stop) break;
        }
    }
    return dp;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, n;
    cin >> m >> n;

    vector<pair<int, int>> producers(m), consumers(n);
    for (auto &[p, d] : producers) cin >> p >> d;
    for (auto &[q, e] : consumers) cin >> q >> e;

    sort(producers.begin(), producers.end());
    vector<pair<int, int>> P;
    for (int d_min = INT_MAX; auto [p, d] : producers)
        if (d_min > d) {
            d_min = d;
            P.emplace_back(p, d);
        }

    sort(consumers.rbegin(), consumers.rend());
    vector<pair<int, int>> C;
    for (int e_max = -1; auto [q, e] : consumers)
        if (e_max < e) {
            e_max = e;
            C.emplace_back(q, e);
        }
    reverse(C.begin(), C.end());

    vector<pair<int, int>> temp;
    for (int i = 0; auto [p, d] : P) {
        for (; i < C.size() && C[i].first <= p; i++);
        if (i < C.size() && C[i].second > d) temp.emplace_back(p, d);
    }
    P = temp;

    if (P.empty()) {
        cout << 0;
        exit(0);
    }

    auto get = [&](int row, int col) -> long long {
        auto [p, d] = P[row];
        auto [q, e] = C[col];
        return (long long) (q - p) * (e - d);
    };

    auto cmp = [&](const auto &p1, const auto &p2) {
        return p1.first < p2.first;
    };

    auto profit = 0LL;
    for (auto [value, col] : smawk<long long>(P.size(), C.size(), get, cmp)) profit = max(profit, value);
    cout << profit;
}