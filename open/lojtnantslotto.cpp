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
vector<T> min_plus_convolve(const vector<T> &concave, const vector<T> &arbitrary) {
    int da = concave.size(), db = arbitrary.size(), n = da + db - 1;

    auto get = [&](int row, int col) -> T {
        if (!(col <= row && row < col + da)) return numeric_limits<T>::max();
        return arbitrary[col] + concave[row - col];
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

    int n, t;
    cin >> n >> t;

    vector<vector<int>> guards(6);
    while (n--) {
        int l, g;
        cin >> l >> g;

        if (g <= t) guards[l].emplace_back(g);
    }

    int riches = 0;
    vector<vector<int>> convex(6);
    for (int l = 1; l <= 5; l++) {
        sort(guards[l].begin(), guards[l].end());
        convex[l].emplace_back(0);
        for (int sum = 0; int g : guards[l]) {
            if (sum + g > t) break;
            sum += g;
            convex[l].emplace_back(sum);
        }
        riches += l * (convex[l].size() - 1);
    }
    riches = min(riches, 5 * t);

    vector<int> dp(riches + 1, t + 1), arbitrary(riches + 1);
    dp[0] = 0;
    for (int l = 1; l <= 5; l++) {
        if (convex[l].size() == 1) continue;

        for (int r = 0, i = 0; r < l; r++) {
            int delta = r * (-t - 1);
            for (int j = r; j <= riches; j += l) arbitrary[i++] = dp[j] + delta;
        }

        auto c = min_plus_convolve(convex[l], arbitrary);

        for (int r = 0, i = 0; r < l; r++) {
            int delta = r * (-t - 1);
            for (int j = r; j <= riches; j += l) dp[j] = c[i++] - delta;
        }
    }

    for (; ~riches; riches--)
        if (dp[riches] <= t) {
            cout << riches;
            break;
        }
}