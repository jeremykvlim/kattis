#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t, n;
    cin >> t >> n;

    vector<vector<pair<long long, int>>> days(5, vector<pair<long long, int>>(t, {1e18, 0}));
    while (n--) {
        int x;
        long long l;
        int p;
        cin >> x >> l >> p;

        if (days[p - 1][x].first > l + x) {
            days[p - 1][x].first = l + x;
            days[p - 1][x].second = (l + x + t) % t;
        }
    }

    vector<vector<pair<long long, int>>> exact(5, vector<pair<long long, int>>(t, {1e18, 0}));
    vector<pair<long long, int>> pref(t, {1e18, 0}), suff(t + 1, {1e18, 0});
    for (int p = 0; p <= 4; p++) {
        pref[0] = days[p][0];
        suff[t] = {1e18, 0};
        for (int x = 1; x < t; x++) pref[x] = min(pref[x - 1], days[p][x]);
        for (int x = t - 1; ~x; x--) suff[x] = min(suff[x + 1], days[p][x]);

        for (int x = 0; x < t; x++) {
            if (suff[x].first < 1e18) {
                suff[x].first -= x;
                exact[p][x] = min(exact[p][x], suff[x]);
            }

            if (x && pref[x - 1].first < 1e18) {
                pref[x - 1].first += t - x;
                exact[p][x] = min(exact[p][x], pref[x - 1]);
            }
        }
    }

    vector<vector<pair<long long, int>>> f(9, vector<pair<long long, int>>(t, {1e18, 0}));
    for (int x = 0; x < t; x++) f[0][x] = {0, x};
    for (int m = 1; m <= 8; m++)
        for (int x = 0; x < t; x++)
            for (int p = 0; p <= 4; p++) {
                if (exact[p][x].first >= 1e18) continue;

                int k = m - p - 1;
                if (k <= 0) f[m][x] = min(f[m][x], exact[p][x]);
                else {
                    if (f[k][exact[p][x].second].first >= 1e18) continue;
                    f[m][x] = min(f[m][x], {exact[p][x].first + f[k][exact[p][x].second].first, f[k][exact[p][x].second].second});
                }
            }

    int lg = 40;
    const auto id = [&](int k, int l, int r) {
        return (k * 9 + l + 4) * t + r;
    };
    vector<pair<long long, int>> g(9 * t * lg, {1e18, 0});
    for (int o = -4; o <= 4; o++)
        for (int x = 0; x < t; x++) g[id(0, o, x)] = o + 1 <= 0 ? make_pair(0LL, x) : f[o + 1][x];

    for (int i = 1; i < lg; i++)
        for (int x = 0; x < t; x++)
            for (int o = -4; o <= 4; o++)
                for (int neg = -4; neg <= 0; neg++)
                    for (int pos = 0; pos <= 4; pos++) {
                        int a = neg + pos, b = o - a;
                        if (a < -4 || a > 4 || b < -4 || b > 4) continue;

                        auto [d1, x1] = g[id(i - 1, a, x)];
                        if (d1 >= 1e18) continue;
                        auto [d2, x2] = g[id(i - 1, b, x1)];
                        if (d2 >= 1e18) continue;

                        g[id(i, o, x)] = min(g[id(i, o, x)], {d1 + d2, x2});
                    }

    int q;
    cin >> q;

    while (q--) {
        long long z;
        cin >> z;

        auto profit = 0LL;
        vector<pair<long long, int>> curr(9, {1e18, 0});
        curr[4] = {0, 0};
        for (int i = lg - 1; ~i; i--) {
            vector<pair<long long, int>> temp1(5, {1e18, 0});
            for (int p = 0; p <= 4; p++)
                for (int o = 0; o <= 8; o++) {
                    auto [d, x] = curr[o];
                    if (d >= 1e18) continue;

                    int k = p - o + 4;
                    if (k <= 0) temp1[p] = min(temp1[p], {d, x});
                    else {
                        if (f[k][x].first >= 1e18) continue;
                        temp1[p] = min(temp1[p], {d + f[k][x].first, f[k][x].second});
                    }
                }

            vector<pair<long long, int>> temp2(9, {1e18, 0});
            for (int o = -4; o <= 4; o++)
                for (int p = 0; p <= 4; p++) {
                    int c = o - p;
                    if (c < -4 || c > 4) continue;

                    auto [d1, x1] = temp1[p];
                    if (d1 >= 1e18) continue;
                    auto [d2, x2] = g[id(i, c, x1)];
                    if (d2 >= 1e18) continue;

                    temp2[o + 4] = min(temp2[o + 4], {d1 + d2, x2});
                }

            if ((*min_element(temp2.begin() + 4, temp2.end())).first <= z) {
                profit += 1LL << i;
                curr = temp2;
            }
        }

        cout << profit << "\n";
    }
}
