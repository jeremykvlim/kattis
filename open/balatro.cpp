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

    vector<int> muls;
    array<array<int, 1001>, 30> adds{};
    array<int, 30> add_count{};
    for (int _ = 0, block = 0; _ < n; _++) {
        char c;
        int v;
        cin >> c >> v;

        if (c == 'a') {
            adds[block][v]++;
            add_count[block]++;
        } else {
            muls.emplace_back(v);
            block++;
        }
    }

    int m = muls.size();
    k = min(k, m);
    vector<long long> concave;
    vector<vector<long long>> dp(k + 1);
    dp[0].emplace_back(0);
    for (int block = 0; block <= m; block++) {
        int s = add_count[block];
        if (s) {
            concave.resize(s + 1);
            concave[0] = 0;
            for (int v = 1e3, j = 0; v > 1; v--)
                for (int count = adds[block][v]; count--; j++) concave[j + 1] = concave[j] + v;
            for (int i = 0; i <= min(block, k); i++) dp[i] = max_plus_convolve(concave, dp[i]);
        }
        if (block == m) break;

        if (block < k) dp[block + 1] = dp[block];
        int r = min(block + 1, k);
        for (int i = 0; i <= r; i++) dp[i].emplace_back(dp[i].back());
        for (int i = r; i; i--)
            for (int j = 1; j < dp[0].size(); j++) dp[i][j] = max(dp[i][j], dp[i - 1][j - 1] * muls[block]);
    }
    for (int i = 1; i <= n; i++) cout << dp[k][i] << "\n";
}
