#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    for (int u = 1; u <= n; u++) {
        int m;
        double x, y;
        cin >> m >> x >> y;

        int max_l = 0;
        vector<int> l(m);
        for (int &li : l) {
            cin >> li;

            max_l = max(max_l, li);
        }

        vector<vector<double>> C(m, vector<double>(m));
        for (auto &row : C)
            for (auto &Cab : row) cin >> Cab;

        int t;
        cin >> t;

        vector<vector<double>> rates(t, vector<double>(m));
        for (auto &row : rates)
            for (auto &r : row) cin >> r;

        vector<vector<vector<pair<double, pair<int, int>>>>> dp(t + 2, vector<vector<pair<double, pair<int, int>>>>(m, vector<pair<double, pair<int, int>>>(max_l, {-1, {0, 0}})));
        for (int i = 0; i < m; i++) dp[0][i][l[i] - 1].first = x;

        double debt = y + 1;
        int months = 1, alt = -1, bind = -1;
        for (; debt > y; months++) {
            for (int i = 0; i < m; i++)
                for (int j = 0; j < max_l; j++) dp[months][i][j].first = -1;

            for (int i = 0; i < m; i++) {
                if (dp[months - 1][i][0].first >= 0)
                    for (int j = 0; j < m; j++) {
                        auto d = floor(100 * (dp[months - 1][i][0].first * (1 + rates[months - 1][i] / 100) + C[i][j])) / 100;
                        if (d <= y) {
                            if (debt > d) {
                                debt = d;
                                alt = i;
                                bind = 0;
                            }
                        } else {
                            d -= y;
                            if (dp[months][j][l[j] - 1].first == -1 || d < dp[months][j][l[j] - 1].first) {
                                dp[months][j][l[j] - 1].first = d;
                                dp[months][j][l[j] - 1].second = {i, 0};
                            }
                        }
                    }

                for (int j = 1; j < l[i]; j++)
                    if (dp[months - 1][i][j].first >= 0) {
                        auto d = floor(100 * (dp[months - 1][i][j].first * (1 + rates[months - 1][i] / 100))) / 100;
                        if (d <= y) {
                            if (debt > d) {
                                debt = d;
                                alt = i;
                                bind = j;
                            }
                        } else {
                            d -= y;
                            if (dp[months][i][j - 1].first == -1 || d < dp[months][i][j - 1].first) {
                                dp[months][i][j - 1].first = d;
                                dp[months][i][j - 1].second = {i, j};
                            }
                        }
                    }
            }
        }
        months -= 2;

        vector<int> w(months + 1);
        w[months] = alt;
        for (int time = months; time; time--) {
            auto [i, j] = dp[time][alt][bind].second;
            w[time - 1] = alt = i;
            bind = j;
        }
        cout << "Test case " << u << "\n";
        for (int v = 0; v <= months; v++) cout << "Month " << v + 1 << ": Alternative " << w[v] + 1 << "\n";
        cout << "Total: " << fixed << setprecision(2) << months * y + debt << "\n";
    }
}
