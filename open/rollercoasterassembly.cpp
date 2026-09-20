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

    int n;
    cin >> n;

    vector<int> a(n), b(n);
    for (int i = 0; i < n; i++) cin >> a[i] >> b[i];

    vector<long long> pref(n, 0);
    for (int i = 1; i < n; i++) pref[i] = pref[i - 1] + min(a[i], b[i - 1]);

    vector<int> indices_a(n - 1), indices_b(n - 1);
    iota(indices_a.begin(), indices_a.end(), 0);
    iota(indices_b.begin(), indices_b.end(), 2);
    sort(indices_a.begin(), indices_a.end(), [&](int i, int j) { return a[i] != a[j] ? a[i] < a[j] : i < j; });
    sort(indices_b.begin(), indices_b.end(), [&](int i, int j) { return b[i - 1] != b[j - 1] ? b[i - 1] < b[j - 1] : i < j; });

    vector<long long> dp(n + 1, numeric_limits<long long>::lowest() / 4);
    dp[0] = 0;
    auto dnc = [&](auto &&self, int l, int r, const auto &indices_a, const auto &indices_b) -> void {
        if (l + 1 == r) return;

        int m = l + (r - l) / 2;
        vector<int> indices_al, indices_ar, indices_bl, indices_br;
        for (int i : indices_a)
            if (i < m - 1) indices_al.emplace_back(i);
            else if (i >= m) indices_ar.emplace_back(i);
        for (int i : indices_b)
            if (i < m) indices_bl.emplace_back(i);
            else indices_br.emplace_back(i);
        self(self, l, m, indices_al, indices_bl);

        if (!indices_al.empty()) {
            auto get = [&](int row, int col) {
                int j = indices_br[row], i = indices_al[col];
                return dp[i] - pref[i] + min(a[i], b[j - 1]);
            };

            auto cmp = [&](const auto &p1, const auto &p2) {
                return p1.first <= p2.first;
            };
            auto rows = smawk<long long>(indices_br.size(), indices_al.size(), get, cmp);
            for (int k = 0; k < indices_br.size(); k++) {
                int j = indices_br[k];
                dp[j] = max(dp[j], pref[j - 1] + rows[k].first);
            }
        }

        int i = m - 1;
        for (int j = m + 1; j < r; j++) dp[j] = max(dp[j], dp[i] + pref[j - 1] - pref[i] + min(a[i], b[j - 1]));
        self(self, m, r, indices_ar, indices_br);
    };
    dnc(dnc, 0, n + 1, indices_a, indices_b);
    cout << dp[n];
}