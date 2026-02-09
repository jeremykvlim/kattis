#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, d, r;
    cin >> N >> d >> r;

    vector<vector<double>> C(N + 1, vector<double>(N + 1));
    for (int n = 0; n <= N; n++) {
        C[n][0] = C[n][n] = 1;
        for (int k = 1; k < n; k++) C[n][k] = C[n - 1][k - 1] + C[n - 1][k];
    }

    vector<vector<double>> dp1(d + 1, vector<double>(N + 1, 0)), dp2(d + 1, vector<double>(N + 1, 0));
    dp1[0][N] = r;
    dp2[0][N] = 1;
    for (int i = 0; i <= d; i++)
        for (int n = 1; n <= N; n++)
            if (dp2[i][n])
                for (int k = 1; k <= min(n, d - i); k++) {
                    dp1[i + k][k] += dp1[i][n] * C[n][k] + dp2[i][n] * C[n][k] * min(k, r);
                    dp2[i + k][k] += dp2[i][n] * C[n][k];
                }

    double numer = 0, denom = 0;
    for (int k = 1; k <= N; k++) {
        numer += dp1[d][k];
        denom += dp2[d][k];
    }
    cout << fixed << setprecision(6) << numer / denom;
}
