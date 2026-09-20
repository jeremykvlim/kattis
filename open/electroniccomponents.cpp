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
vector<T> min_plus_convolve(const vector<T> &convex, const vector<T> &arbitrary) {
    int da = convex.size(), db = arbitrary.size(), n = da + db - 1;

    auto get = [&](int row, int col) -> T {
        if (!(col <= row && row < col + da)) return numeric_limits<T>::max();
        return arbitrary[col] + convex[row - col];
    };

    auto cmp = [&](const auto &p1, const auto &p2) {
        return p1.first >= p2.first;
    };
    auto dp = smawk<T>(n, db, get, cmp);

    vector<T> c(n);
    for (int i = 0; i < n; i++) c[i] = dp[i].first;
    return c;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, long long>> components(n);
    int most = 0;
    for (auto &[f, t] : components) {
        cin >> f >> t;

        most = max(most, f);
    }
    sort(components.begin(), components.end(), [&](auto p1, auto p2) { return p1.second > p2.second; });

    int parity = 0;
    vector<long long> dp(2 * most + 1, 1e18), temp(2 * most + 1);
    dp[0] = 0;
    for (auto [f, t] : components) {
        vector<long long> convex(f + 1), arbitrary;
        for (int i = 0; i <= f; i++) convex[i] = t * i;
        for (int i = parity; i <= 2 * most; i += 2) arbitrary.emplace_back(dp[i]);
        auto c = min_plus_convolve(convex, arbitrary);

        parity ^= f & 1;
        fill(temp.begin(), temp.end(), 1e18);
        auto least = LLONG_MAX;
        for (int i = parity; i < f; i += 2) {
            int l = f - i, r = f + i;
            least = min({least, 2 * dp[l] - t * l, 2 * dp[r] - t * r});
            if (least != LLONG_MAX) temp[i] = (t * (f + i) + least) / 2;
        }
        for (int i = f + (parity ^ (f & 1)); i <= 2 * most; i += 2) temp[i] = c[(i + f - (parity ^ (f & 1))) / 2];
        dp = temp;
    }
    cout << *min_element(dp.begin(), dp.end());
}