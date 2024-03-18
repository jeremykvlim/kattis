#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    for (int x = 0; x < t; x++) {
        int n;
        cin >> n;

        vector<double> p(n);
        double sum_p = 0;
        for (int i = 0; i < n; i++) {
            cin >> p[i];
            sum_p += p[i];
        }

        double curr = 2 * sum_p / n, prev = 201;
        while (curr < prev) {
            double sum_s = 0, count = 0;
            for (auto v : p)
                if (v <= curr) {
                    sum_s += v;
                    count++;
                }

            prev = curr;
            curr = (sum_p + sum_s) / count;
        }

        cout << "Case #" << x + 1 << ":";
        for (auto v : p) cout << " " << fixed << setprecision(5) << max((curr - v) / sum_p * 100, 0.0);
        cout << "\n";
    }
}
