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

    int l;
    cin >> l;

    vector<int> p(l), d(l - 1);
    for (int &pi : p) cin >> pi;
    for (int &di : d) cin >> di;

    int t, n;
    cin >> t >> n;

    d.emplace_back(t + 1);
    vector<int> pref(t + 1, 0);
    for (int i = 1, j = 0, days = d[0]; i <= t; i++) {
        pref[i] = pref[i - 1] + p[j];

        if (!--days && j + 1 < l) {
            j++;
            days = d[j];
        }
    }

    vector<int> a(n), b(n), dp(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i] >> b[i];

        dp[i] = pref[a[i] - 1];
    }

    auto dnc = [&](auto &&self, int l, int r) -> void {
        if (l + 1 == r) return;

        int m = l + (r - l) / 2;
        self(self, l, m);

        auto get = [&](int row, int col) {
            int i = m + row, j = m - 1 - col;
            return dp[j] + pref[a[i] - b[j] - 1];
        };

        auto cmp = [&](const auto &p1, const auto &p2) {
            return p1.first > p2.first;
        };
        auto rows = smawk<int>(r - m, m - l, get, cmp);
        for (int i = m; i < r; i++) dp[i] = min(dp[i], rows[i - m].first);

        self(self, m, r);
    };
    if (n) dnc(dnc, 0, n);

    for (int i = 0; i < n; i++) pref[t] = min(pref[t], dp[i] + pref[t - b[i]]);
    cout << pref[t];
}