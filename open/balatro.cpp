#include <bits/stdc++.h>
using namespace std;

template <typename T>
vector<T> min_plus_convolution(const vector<T> &convex, const vector<T> &arbitrary){
    int da = convex.size(), db = arbitrary.size(), n = da + db - 1;

    vector<T> c(n, numeric_limits<T>::max() / 4);
    c[0] = convex[0] + arbitrary[0];
    vector<int> indices(n + 1, 0);
    indices[n] = db - 1;
    for (int k = bit_ceil((unsigned) n) >> 1; k; k >>= 1)
        for (int i = k; i < n; i += k << 1) {
            int l = i - k, r = min(i + k, n);
            indices[i] = indices[l];
            for (int j = indices[l]; j <= indices[r]; j++) {
                if (j <= i && i < j + da) {
                    T t = arbitrary[j] + convex[i - j];
                    if (c[i] > t) {
                        c[i] = t;
                        indices[i] = j;
                    }
                }
            }
        }
    return c;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<pair<char, int>> cards(n);
    for (auto &[c, v] : cards) cin >> c >> v;

    vector<int> muls, add;
    vector<vector<int>> adds;
    for (auto [c, v] : cards)
        if (c == 'a') add.emplace_back(v);
        else {
            adds.emplace_back(add);
            add.clear();
            muls.emplace_back(v);
        }
    adds.emplace_back(add);

    int m = muls.size();
    k = min(k, m);

    vector<long long> temp;
    vector<vector<long long>> dp(k + 1, {(long long) -1e18});
    dp[0][0] = 0;
    for (int block = 0; block <= m; block++) {
        if (!adds[block].empty()) {
            sort(adds[block].rbegin(), adds[block].rend());

            int s = adds[block].size();
            vector<long long> pref(s + 1, 0);
            for (int i = 0; i < s; i++) pref[i + 1] = pref[i] + adds[block][i];

            vector<long long> a(s + 1);
            for (int i = 0; i <= s; i++) a[i] = -pref[i];

            for (int i = 0; i <= min(block, k); i++) {
                vector<long long> b(dp[i].size());
                for (int j = 0; j < dp[i].size(); j++) b[j] = -dp[i][j];

                auto c = min_plus_convolution(a, b);
                temp.resize(min((int) c.size(), n - i + 1));
                for (int j = 0; j < temp.size(); j++) temp[j] = -c[j];
                dp[i] = temp;
            }
        }

        if (block < m && k)
            for (int i = min(block, k - 1); ~i; i--) {
                dp[i + 1].resize(min((int) dp[i].size(), n - i), -1e18);
                for (int j = 0; j < dp[i + 1].size(); j++)
                    if (dp[i][j] != -1e18) dp[i + 1][j] = max(dp[i + 1][j], dp[i][j] * muls[block]);
            }
    }

    vector<long long> score(n + 1, 0);
    for (int i = 0; i <= k; i++)
        for (int j = 0; j <= n - i && j < dp[i].size(); j++) score[i + j] = max(score[i + j], dp[i][j]);

    for (int i = 1; i <= n; i++) {
        score[i] = max(score[i], score[i - 1]);
        cout << score[i] << "\n";
    }
}
