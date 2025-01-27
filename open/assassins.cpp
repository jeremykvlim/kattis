#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<double> alive(1 << n, 0);
    alive[(1 << n) - 1] = 1;

    while (m--) {
        int i, j;
        double p;
        cin >> i >> j >> p;

        int mask = 0;
        while (mask < 1 << n) {
            mask |= (1 << (i - 1)) | (1 << (j - 1));
            alive[mask & ~(1 << (j - 1))] += p * alive[mask];
            alive[mask++] *= (1 - p);
        }
    }

    vector<double> total(n, 0);
    for (int mask = 0; mask < 1 << n; mask++)
        for (int i = 0; i < n; i++)
            if ((mask >> i) & 1) total[i] += alive[mask];

    for (auto p : total) cout << fixed << setprecision(6) << p << "\n";
}
