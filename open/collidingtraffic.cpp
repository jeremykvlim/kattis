#include <bits/stdc++.h>
using namespace std;

constexpr double EPSILON = 1e-6;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int c;
    cin >> c;

    while (c--) {
        int n;
        long double r;
        cin >> n >> r;

        vector<pair<long double, long double>> position(n), velocity(n);
        for (int i = 0; i < n; i++) {
            long double d, s;
            cin >> position[i].first >> position[i].second >> d >> s;

            velocity[i] = {s * cos((90 - d) / 180 * M_PI), s * sin((90 - d) / 180 * M_PI)};
        }

        auto time = LDBL_MAX;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < i; j++) {
                long double t;
                auto boat1 = make_pair(position[i].first - position[j].first, position[i].second - position[j].second),
                         v = make_pair(velocity[i].first - velocity[j].first, velocity[i].second - velocity[j].second);

                if (hypotl(boat1.first, boat1.second) < r + EPSILON) t = 0;
                else if (hypotl(v.first, v.second) < EPSILON) continue;
                else {
                    t = (boat1.first * -v.first + boat1.second * -v.second) / (v.first * v.first + v.second * v.second);
                    if (t + EPSILON < 0) continue;
                    else {
                        auto boat2 = make_pair(boat1.first + v.first * t, boat1.second + v.second * t);

                        if (hypotl(boat2.first, boat2.second) > r) continue;
                        else t -= sqrt(r * r - (boat2.first * boat2.first + boat2.second * boat2.second)) / hypotl(v.first, v.second);
                    }
                }

                time = min(time, t);
            }

        if (time == LDBL_MAX) cout << "No collision.\n";
        else cout << fixed << setprecision(0) << time << "\n";
    }
}
