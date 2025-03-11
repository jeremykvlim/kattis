#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> pos(n);
    for (int &p : pos) cin >> p;

    double dist = max(pos[0], (int) 5e6 - pos[n - 1]);
    for (int i = 1; i < n; i++) dist = max(dist, (pos[i] - pos[i - 1]) / 2.);

    int remove = n, i = 0;
    for (double l = 0, r = 0; i < n && l < 5e6; l = r, remove--) {
        for (; i < n && pos[i] - dist <= l; i++) r = max(r, dist + pos[i]);
        if (l == r) break;
    }

    cout << fixed << setprecision(1) << dist << " " << remove;
}
