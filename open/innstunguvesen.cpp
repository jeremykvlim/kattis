#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    double r;
    cin >> n >> r;

    if (n - r < 2) {
        cout << "Endalaust rafmagn";
        exit(0);
    }

    vector<double> b(n);
    for (auto &bi : b) cin >> bi;
    sort(b.begin(), b.end());
    b.emplace_back(1e20);

    double pref = 0;
    for (int i = 0; i < n; i++) {
        pref += b[i];
        if (i > r) {
            auto t = pref / (i - r);
            if (b[i] <= t && t <= b[i + 1]) {
                cout << fixed << setprecision(6) << t;
                exit(0);
            }
        }
    }
}
