#include <bits/stdc++.h>
#include <tr2/dynamic_bitset>
using namespace std;
using namespace tr2;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<int> total(n, 0);
    vector<vector<int>> a(n, vector<int>(m));
    for (int i = 0; i < n; i++)
        for (int &aij : a[i]) {
            cin >> aij;

            total[i] += aij;
        }

    vector<int> indices(n);
    iota(indices.begin(), indices.end(), 0);
    sort(indices.begin(), indices.end(), [&](int i, int j) { return total[i] != total[j] ? total[i] > total[j] : i < j; });

    vector<int> dp(m + 1, -1);
    dp[0] = 1e9;

    for (int x = 0; x < n; x++) {
        int i = indices[x], r = max(0, min(m, ((x ? total[indices[x - 1]] : (int) 1e9) - (x + 1 < n ? total[indices[x + 1]] : 0)) / k)), score = 0;
        if (r) {
            nth_element(a[i].begin(), a[i].begin() + r, a[i].end(), greater<>());
            for (int j = 0; j < r; j++) score += a[i][j];
        }

        vector<dynamic_bitset<>> bs(r + 1, dynamic_bitset<>(score + 1));
        bs[0][0] = true;
        for (int j = 0; j < m; j++)
            for (int p = r; p; p--) bs[p] |= bs[p - 1] << a[i][j];

        vector<int> temp(m + 1, -1);
        for (int q = 0; q <= m; q++)
            if (~dp[q])
                for (int p = 0; p <= min(m - q, r); p++) {
                    int s = max(0, total[i] + k * p - dp[q] + (x && indices[x - 1] > i));
                    if (s <= score && !bs[p][s]) s = bs[p].find_next(s);
                    if (s <= score) temp[p + q] = max(temp[p + q], total[i] - s);
                }
        dp = temp;
    }
    reverse(dp.begin(), dp.end());
    cout << n * m - (m - (find_if(dp.begin(), dp.end(), [](int s) { return s >= 0; }) - dp.begin()));
}
