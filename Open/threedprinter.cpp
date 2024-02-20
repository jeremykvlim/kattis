#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    double vol = 0;
    while (n--) {
        int f;
        cin >> f;

        double x_start = 101, y_start = 101, z_start = 101;
        while (f--) {
            int v;
            cin >> v;

            double x_first = 101, y_first = 101, z_first = 101, x_prev, y_prev, z_prev;
            while (v--) {
                double x, y, z;
                cin >> x >> y >> z;

                if (x_first == 101) {
                    tie(x_first, y_first, z_first) = tie(x, y, z);

                    if (x_start == 101) tie(x_start, y_start, z_start) = tie(x, y, z);
                } else {
                    double x_cross = (y_prev - y_first) * (z - z_first) - (z_prev - z_first) * (y - y_first),
                           y_cross = (z_prev - z_first) * (x - x_first) - (x_prev - x_first) * (z - z_first),
                           z_cross = (x_prev - x_first) * (y - y_first) - (y_prev - y_first) * (x - x_first);

                    vol += abs((x_first - x_start) * x_cross + (y_first - y_start) * y_cross + (z_first - z_start) * z_cross) / 6;
                }

                tie(x_prev, y_prev, z_prev) = tie(x, y, z);
            }
        }
    }

    cout << fixed << setprecision(2) << vol;
}
