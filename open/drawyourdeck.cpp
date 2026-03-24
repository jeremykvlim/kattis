#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> a(k + 1);
    for (int &ai : a) cin >> ai;
    a.resize(4, 0);

    int sum = 0;
    for (int i = 0; i <= k; i++) sum += (i - 1) * a[i];

    auto base = 1 - (sum == -1 ? (double) a[1] / n : 0);
    vector<vector<double>> dp1(a[0] + 1, vector<double>(a[2] + 1, 0)), dp2(a[0] + 1, vector<double>(a[2] + 1, 0));
    for (int z = 0; z <= a[3]; z++) {
        for (int x = 0; x <= a[0]; x++) {
            if (!x && !z) dp2[x][0] = base;
            for (int y = 0; y <= min(a[2], sum + x - 2 * z); y++)
                if (x || y || z) dp2[x][y] += (x ? (double) x / (x + y + z) * dp2[x - 1][y] : 0) + (y ? (double) y / (x + y + z) * dp2[x][y - 1] : 0) + (z ? (double) z / (x + y + z) * dp1[x][y] : 0);
        }
        swap(dp1, dp2);
        for (auto &row : dp2) fill(row.begin(), row.end(), 0);
    }
    cout << fixed << setprecision(6) << dp1[a[0]][a[2]];
}
