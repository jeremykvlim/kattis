#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    long long l;
    cin >> n >> k >> l;

    vector<long long> m(n), x(n);
    vector<int> T(n);
    int t_max = -1;
    for (int i = 0; i < n; i++) {
        cin >> m[i] >> x[i] >> T[i];

        t_max = max(t_max, T[i]);
    }

    vector<int> indices(n);
    iota(indices.begin(), indices.end(), 0);
    sort(indices.begin(), indices.end(), [&](int i, int j) { return m[i] < m[j]; });
    int trauma = INT_MAX, c = n * t_max;
    vector<long long> dp(c + 1);
    for (int t = 0; t <= t_max; t++) {
        fill(dp.begin(), dp.end(), -1);
        dp[0] = 0;
        for (int bound = 0; int i : indices) {
            int cost = max(0, T[i] - t);
            for (int j = min(bound, c - cost); j >= 0; j--)
                if (dp[j] >= m[i]) dp[j + cost] = max(dp[j + cost], dp[j] + x[i]);
            bound = min(bound + cost, c);
        }

        int j = find_if(dp.begin(), dp.end(), [&](auto level) { return level >= l; }) - dp.begin();
        if (j > c) continue;
        trauma = min(trauma, j + k * t);
    }
    cout << trauma;
}
