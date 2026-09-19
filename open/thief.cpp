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

template <typename T>
vector<T> max_plus_convolve(const vector<T> &concave, const vector<T> &arbitrary) {
    int da = concave.size(), db = arbitrary.size(), n = da + db - 1;

    auto get = [&](int row, int col) -> T {
        if (!(col <= row && row < col + da)) return numeric_limits<T>::lowest();
        return arbitrary[col] + concave[row - col];
    };

    auto cmp = [&](const auto &p1, const auto &p2) {
        return p1.first <= p2.first;
    };
    auto dp = smawk<T>(n, db, get, cmp);

    vector<T> c(n);
    for (int i = 0; i < n; i++) c[i] = dp[i].first;
    return c;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    auto total = 1LL;
    vector<vector<int>> jewels(301);
    while (n--) {
        int s, v;
        cin >> s >> v;

        if (s <= k) {
            jewels[s].emplace_back(v);
            total += v;
        }
    }

    vector<long long> dp(k + 1, 0), concave, arbitrary(k + 1);
    for (int s = 1; s <= min(k, 300); s++) {
        if (jewels[s].empty()) continue;

        int steal = min((int) jewels[s].size(), k / s);
        partial_sort(jewels[s].begin(), jewels[s].begin() + steal, jewels[s].end(), greater<>());
        jewels[s].resize(steal);
        concave.resize(steal + 1);
        concave[0] = 0;
        for (int i = 0; i < steal; i++) concave[i + 1] = concave[i] + jewels[s][i];

        for (int r = 0, i = 0; r < s; r++) {
            auto delta = total * r;
            for (int j = r; j <= k; j += s) arbitrary[i++] = dp[j] + delta;
        }

        auto c = max_plus_convolve(concave, arbitrary);

        for (int r = 0, i = 0; r < s; r++) {
            auto delta = total * r;
            for (int j = r; j <= k; j += s) dp[j] = c[i++] - delta;
        }
    }

    for (int i = 1; i <= k; i++) cout << dp[i] << " ";
}