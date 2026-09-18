#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t, n;
    cin >> t >> n;

    vector<vector<long long>> days(t, vector<long long>(5, 1e18));
    while (n--) {
        int x, p;
        long long l;
        cin >> x >> l >> p;

        days[x][p - 1] = min(days[x][p - 1], x + l);
    }

    vector<long long> pref(t), suff(t + 1);
    vector<vector<long long>> exact(t, vector<long long>(5, 1e18));
    for (int p = 0; p < 5; p++) {
        pref[0] = days[0][p];
        for (int x = 1; x < t; x++) pref[x] = min(pref[x - 1], days[x][p]);
        suff[t] = 1e18;
        for (int x = t - 1; ~x; x--) suff[x] = min(suff[x + 1], days[x][p]);

        for (int x = 0; x < t; x++) {
            if (suff[x] != 1e18) exact[x][p] = suff[x] - x;
            if (x && pref[x - 1] != 1e18) exact[x][p] = min(exact[x][p], pref[x - 1] + t - x);
        }
    }

    vector<vector<long long>> f(t, vector<long long>(5, 1e18));
    for (int m = 0; m < 5; m++)
        for (int x = 0; x < t; x++)
            for (int p = 0; p < 5; p++) {
                auto y = exact[x][p];
                if (y != 1e18)
                    f[x][m] = min(f[x][m], (p < m ? f[(x + y % t) % t][m - p - 1] : 0) + y);
            }

    vector<vector<vector<long long>>> g(40, vector<vector<long long>>(t, vector<long long>(9, 1e18)));
    for (int x = 0; x < t; x++)
        for (int o = -4; o <= 4; o++) g[0][x][o + 4] = o + 1 <= 0 ? 0 : f[x][o];

    for (int k = 1; k < 40; k++)
        for (int x = 0; x < t; x++)
            for (int i = 0; i < 9; i++) {
                auto y = g[k - 1][x][i];
                if (y != 1e18)
                    for (int j = max(0, 4 - i); j <= min(8, 12 - i); j++) g[k][x][i + j - 4] = min(g[k][x][i + j - 4], g[k - 1][(x + y % t) % t][j] + y);
            }

    int q;
    cin >> q;

    while (q--) {
        long long z;
        cin >> z;

        vector<long long> curr(9, 0), next(9);
        for (int o = -4; o <= 4; o++) curr[o + 4] = o <= 0 ? 0 : f[0][o - 1];

        auto profit = 0LL;
        for (int k = 39; ~k; k--) {
            fill(next.begin(), next.end(), 1e18);
            for (int i = 0; i < 9; i++) {
                auto y = curr[i];
                if (y != 1e18)
                    for (int j = max(0, 4 - i); j <= min(8, 12 - i); j++) next[i + j - 4] = min(next[i + j - 4], g[k][y % t][j] + y);
            }

            if (z >= next[4]) {
                profit += 1LL << k;
                curr = next;
            }
        }
        cout << profit << "\n";
    }
}
