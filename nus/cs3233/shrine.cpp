#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int w;
    while (cin >> w && w) {
        int n, D;
        cin >> n >> D;

        vector<int> shrines;
        while (D--) {
            int d;
            cin >> d;

            for (int i = d; i <= n; i += d) shrines.emplace_back(i);
        }
        sort(shrines.begin(), shrines.end());
        shrines.erase(unique(shrines.begin(), shrines.end()), shrines.end());

        if (shrines.size() <= w) {
            cout << "2000.0\n";
            continue;
        }

        int s = shrines.size();
        double R = 1e3;
        auto law_of_cosines = [&](int d) {
            return sqrt(R * R + R * R - 2 * R * R * cos(2 * M_PI * d / n));
        };

        vector<double> dist{law_of_cosines(shrines[0])};
        for (int i = 0; i < s - 1; i++) dist.emplace_back(law_of_cosines(shrines[i + 1] - shrines[i]));

        double l = 2 * R, r = 2 * M_PI * R + 2 * R, m;
        while (l + 1e-6 < r && l + l * 1e-6 < r) {
            m = l + (r - l) / 2;

            auto valid = [&]() {
                double d = 2 * R;
                int i = 0;
                while (i < s && d + dist[i] <= m) {
                    d += dist[i];
                    i++;
                }
                if (i == s) return true;

                for (int j = 0; j <= i; j++) {
                    d = 2 * R;
                    for (int k = j, count = 0; count < w;) {
                        if (d + dist[k] <= m) d += dist[k];
                        else {
                            d = 2 * R;
                            count++;
                        }
                        k = (k + 1) % s;
                        if (k == j) return true;
                    }
                }
                return false;
            };

            if (valid()) r = m;
            else l = m;
        }
        cout << fixed << setprecision(1) << r << "\n";
    }
}
