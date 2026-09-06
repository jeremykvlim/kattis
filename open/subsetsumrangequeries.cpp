#pragma GCC optimize("Ofast,unroll-loops")
#pragma GCC target("avx2")
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<int> w(n);
    for (int &wi : w) cin >> wi;

    int split = n;
    vector<int> l(q), r(q), T(q);
    for (int i = 0; i < q; i++) {
        cin >> l[i] >> r[i] >> T[i];
        l[i]--;
        r[i]--;

        split = min(split, l[i]);
    }

    split += USHRT_MAX;
    vector<array<int, 2>> queries(n, {-1, -1});
    vector<int> next(q);
    array<int, 2> first{n, n}, last{-1, -1}, t_max{0, 0};
    for (int i = 0; i < q; i++) {
        int k = l[i] >= split;
        next[i] = queries[r[i]][k];
        queries[r[i]][k] = i;
        first[k] = min(first[k], l[i]);
        last[k] = max(last[k], r[i]);
        t_max[k] = max(t_max[k], T[i]);
    }

    vector<bool> can(q);
    alignas(64) array<unsigned short, (int) 1e5 + 1> dp;
    auto solve = [&](int k) {
        if (!~last[k]) return;

        int t = t_max[k];
        fill(dp.begin(), dp.begin() + t + 1, 0);
        for (int r = first[k], sum = 0; r <= last[k]; r++) {
            dp[t] = min(r - first[k] + 1, USHRT_MAX);
            int wi = w[r];
            if (wi <= t) {
                sum = min(t, sum + wi);
                for (int i = t - sum; i <= t - wi; i++) dp[i] = max(dp[i], dp[i + wi]);
            }
            for (int i = queries[r][k]; ~i; i = next[i]) can[i] = dp[t - T[i]] >= l[i] - first[k] + 1;
        }
    };
    for (int k = 0; k < 2; k++) solve(k);
    for (auto b : can) cout << (b ? "YES\n" : "NO\n");
}
