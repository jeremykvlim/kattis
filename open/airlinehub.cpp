#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    while (cin >> n) {
        vector<double> la(n), lo(n), x(n), y(n), z(n);
        for (int i = 0; i < n; i++) {
            cin >> la[i] >> lo[i];

            auto theta = la[i] * M_PI / 180, phi = lo[i] * M_PI / 180;
            x[i] = cos(theta) * cos(phi);
            y[i] = cos(theta) * sin(phi);
            z[i] = sin(theta);
        }

        double dist = -1;
        int k = -1;
        for (int i = 0; i < n; i++) {
            double d = 1;
            for (int j = 0; j < n; j++) d = min(d, x[i] * x[j] + y[i] * y[j] + z[i] * z[j]);

            if (dist <= d) {
                dist = d;
                k = i;
            }
        }
        cout << fixed << setprecision(2) << la[k] << " " << lo[k] << "\n";
    }
}
