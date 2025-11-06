#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto read = [&]() -> pair<int, vector<int>> {
        int V;
        cin >> V;

        vector<int> v(V);
        for (int &vi : v) cin >> vi;
        sort(v.begin(), v.end());
        return {V, v};
    };
    auto [A, a] = read();
    auto [B, b] = read();
    auto [C, c] = read();

    vector<vector<array<double, 3>>> dp1(B + 1, vector<array<double, 3>>(C + 1)), dp2(B + 1, vector<array<double, 3>>(C + 1));
    for (int i = A; ~i; i--) {
        for (int j = B; ~j; j--) {
            for (int k = C; ~k; k--) {
                if (i == A) dp1[j][k] = {1, 0, 0};
                else if (j == B) dp1[j][k] = {0, 1, 0};
                else if (k == C) dp1[j][k] = {0, 0, 1};
                else {
                    double sa = a[i], sb = b[j], sc = c[k], s = sa + sb + sc;
                    for (int l = 0; l < 3; l++) dp1[j][k][l] = (sa / s) * dp2[j][k][l] + (sb / s) * dp1[j + 1][k][l] + (sc / s) * dp1[j][k + 1][l];
                }
            }
        }
        if (i) swap(dp1, dp2);
    }
    cout << fixed << setprecision(6) << dp1[0][0][0] << "\n" << dp1[0][0][1] << "\n" << dp1[0][0][2];
}
