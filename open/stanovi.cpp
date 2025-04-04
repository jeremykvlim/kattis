#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, K;
    cin >> n >> m >> K;

    int l = max(n, m);
    auto deviation = [&](int a, int b) {return (long long) ((a * b) - K) * ((a * b) - K);};

    vector<vector<long long>> dp1(l + 1, vector<long long>(l + 1)),
                              dp2(l + 1, vector<long long>(l + 1)),
                              dp3(l + 1, vector<long long>(l + 1)),
                              dp4(l + 1, vector<long long>(l + 1));
    
    for (int i = 1; i <= l; i++)
        for (int j = 1; j <= l; j++) {
            dp1[i][j] = dp2[i][j] = dp3[i][j] = dp4[i][j] = deviation(i, j);
            for (int k = 1; k < j; k++) dp1[i][j] = min(dp1[i][j], dp1[i][k] + dp1[i][j - k]);
        }

    for (int i = 1; i <= l; i++)
        for (int j = 1; j <= l; j++) {
            for (int k = 1; k < j; k++) {
                dp2[i][j] = min(dp2[i][j], dp2[i][k] + dp2[i][j - k]);
                dp3[i][j] = min(dp3[i][j], dp1[i][k] + dp3[i][j - k]);
                dp4[i][j] = min(dp4[i][j], dp2[i][k] + dp4[i][j - k]);
            }
            for (int k = 1; k < i; k++) {
                dp2[i][j] = min(dp2[i][j], dp1[k][j] + dp1[i - k][j]);
                dp3[i][j] = min(dp3[i][j], dp3[k][j] + dp1[j][i - k]);
                dp4[i][j] = min(dp4[i][j], dp3[k][j] + dp3[i - k][j]);
            }
        }

    auto least = deviation(n, m);
    for (int i = 1; i < n; i++) least = min(least, dp4[m][i] + dp4[m][n - i]);
    for (int i = 1; i < m; i++) least = min(least, dp4[n][i] + dp4[n][m - i]);
    cout << least;
}
