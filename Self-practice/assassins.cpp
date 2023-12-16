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

        int state = 0;
        while (state < 1 << n) {
            state |= (1 << (i - 1)) | (1 << (j - 1));
            alive[state & ~(1 << (j - 1))] += p * alive[state];
            alive[state++] *= (1 - p);
        }
    }

    vector<double> total(n, 0);
    for (int state = 0; state < 1 << n; state++)
        for (int i = 0; i < n; i++)
            if (state & (1 << i)) total[i] += alive[state];

    for (auto p : total) cout << fixed << setprecision(6) << p << "\n";
}
