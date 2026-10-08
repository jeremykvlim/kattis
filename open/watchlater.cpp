#include <bits/stdc++.h>
using namespace std;

template <typename T, typename F>
void fast_subset_transform(int n, vector<T> &v, F &&f) {
    for (int k = 1; k < n; k <<= 1)
        for (int i = 0; i < n; i += k << 1)
            for (int j = 0; j < k; j++) v[i + j + k] = f(v[i + j + k], v[i + j]);
}

template <typename T>
void subset_zeta_transform(int n, vector<T> &f) {
    fast_subset_transform(n, f, [](T x, T y) { return x + y; });
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    string s;
    cin >> n >> k >> s;

    vector<int> node(n), next(26, -1);
    vector<vector<int>> indices(k);
    k = 0;
    for (int i = 0; i < n; i++) {
        int pos = s[i] - 'a';
        if (next[pos] == -1) next[pos] = k++;
        indices[node[i] = next[pos]].emplace_back(i);
    }

    int m = 1 << k, half = m >> 1;
    vector<vector<int>> merges(k, vector<int>(half, 0));
    for (int t = 0; t < k; t++) {
        for (int j = 0; j + 1 < indices[t].size(); j++) {
            int mask = 0;
            for (int i = indices[t][j] + 1; i < indices[t][j + 1]; i++) mask |= 1 << node[i];
            mask = (mask & ((1 << t) - 1)) | ((mask >> (t + 1)) << t);
            merges[t][mask]++;
        }

        subset_zeta_transform(half, merges[t]);
    }

    vector<int> dp(m, 1e9);
    dp[0] = 0;
    for (int m1 = 1; m1 < m; m1++)
        for (int m2 = m1; m2; m2 &= m2 - 1) {
            int t = countr_zero((unsigned) m2), m3 = m1 ^ (1 << t), m4 = (m3 & ((1 << t) - 1)) | ((m3 >> (t + 1)) << t);
            dp[m1] = min(dp[m1], dp[m3] + (int) indices[t].size() - merges[t][m4]);
        }
    cout << dp[m - 1];
}
