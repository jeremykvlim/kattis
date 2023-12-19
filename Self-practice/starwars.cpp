#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    for (int t = 1; t <= T; t++) {
        int n;
        cin >> n;

        vector<double> x(n), y(n), z(n), p(n);
        for (int i = 0; i < n; i++) cin >> x[i] >> y[i] >> z[i] >> p[i];

        double l = 0, r = INT_MAX, m;
        while (l + 1e-9 < r) {
            m = l + (r - l) / 2;
            vector<vector<vector<double>>> ships(2, vector<vector<double>>(2, vector<double>(2, DBL_MAX)));

            for (int i = 0; i < n; i++)
                for (int X = 0; X < 2; X++)
                    for (int Y = 0; Y < 2; Y++)
                        for (int Z = 0; Z < 2; Z++)
                            ships[X][Y][Z] = min(ships[X][Y][Z], m * p[i] + (X ? x[i] : -x[i]) + (Y ? y[i] : -y[i]) + (Z ? z[i] : -z[i]));

            double even = 0, odd = 0;
            for (int X = 0; X < 2; X++)
                for (int Y = 0; Y < 2; Y++)
                    for (int Z = 0; Z < 2; Z++)
                        if (!((X + Y + Z) & 1)) {
                            if (ships[X][Y][Z] + ships[!X][!Y][!Z] < 0) goto next;
                            even += ships[X][Y][Z];
                            odd += ships[!X][!Y][!Z];
                        }

            if (even >= 0 && odd >= 0) r = m;
            else {
                next:;
                l = m;
            }
        }

        cout << fixed << setprecision(6) << "Case #" << t << ": " << m << "\n";
    }
}
