#include <bits/stdc++.h>
using namespace std;

template <typename T, typename F>
void fast_subset_transform(int n, vector<T> &v, F &&f) {
    for (int k = 1; k < n; k <<= 1)
        for (int i = 0; i < n; i += k << 1)
            for (int j = 0; j < k; j++) v[i + j + k] = f(v[i + j + k], v[i + j]);
}

template <typename T>
vector<T> fzt(int n, const vector<T> &f) {
    auto F = f;
    fast_subset_transform(n, F, [](auto x, auto y) { return x + y; });
    return F;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    string s;
    cin >> n >> k >> s;

    k = 0;
    vector<int> node(n), next(26, -1);
    vector<vector<int>> indices(k);
    for (int i = 0; i < n; i++) {
        int pos = s[i] - 'a';
        if (next[pos] == -1) next[pos] = k++;
        indices[node[i] = next[pos]].emplace_back(i);
    }

    int m = 1 << k;
    vector<int> merges(k * m, 0);
    for (int t = 0; t < k; t++)
        for (int j = 0; j + 1 < indices[t].size(); j++) {
            int mask = 0;
            for (int i = indices[t][j] + 1; i < indices[t][j + 1]; i++) mask |= 1 << node[i];
            merges[t * m + mask]++;
        }

    for (int t = 0; t < k; t++) {
        vector<int> temp(m);
        for (int mask = 0; mask < m; mask++) temp[mask] = merges[t * m + mask];
        temp = fzt(m, temp);
        for (int mask = 0; mask < m; mask++) merges[t * m + mask] = temp[mask];
    }

    vector<int> dp(m, 1e9);
    dp[0] = 0;
    for (int mask = 0; mask < m; mask++)
        for (int t = 0; t < k; t++)
            if (!((mask >> t) & 1)) dp[mask | (1 << t)] = min(dp[mask | (1 << t)], dp[mask] + (int) indices[t].size() - merges[t * m + mask]);
    cout << dp[m - 1];
}
