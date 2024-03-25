#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<double> l(n + 1), r(n + 1), m(n + 1), len(n + 1), pref_m(n + 1, 0);
    vector<vector<double>> pref_r(n + 1, vector<double>(4, 0));
    for (int i = 1; i <= n; i++) {
        cin >> l[i] >> r[i];

        len[i] = r[i] - l[i];
        m[i] = l[i] + len[i] / 2;
        pref_m[i] = pref_m[i - 1] + m[i];

        double k = 1;
        for (int j = 0; j < 4; j++, k *= r[i]) pref_r[i][j] = pref_r[i - 1][j] + (len[i] < 1e-9 ? 0 : k / len[i]);
    }

    double damages = 0;
    for (int i = 1; i <= n; i++) {
        damages += (i - 1) * m[i] - pref_m[i - 1];
        if (len[i] < 1e-9) continue;

        double k = 1;
        for (int j = 3, p = upper_bound(r.begin(), r.end(), l[i]) - r.begin(); ~j; j--, k *= l[i]) damages -= (j & 1 ? -1 : 1) * k / len[i] * (pref_r[i - 1][j] - pref_r[p - 1][j]) / (j == 3 || !j ? 3 : 1);
    }

    cout << fixed << setprecision(4) << damages / n / n;
}
