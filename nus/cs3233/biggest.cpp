#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    const int limit = 360 * 60 * 60;
    while (t--) {
        int r, n, a, m, s;
        cin >> r >> n >> a >> m >> s;

        vector<bool> sliced(limit + 1);
        for (int i = 0, pos = 0; i < n && !sliced[pos]; i++) {
            sliced[pos] = true;
            pos += a * 60 * 60 + m * 60 + s;
            if (pos >= limit) pos -= limit;
        }

        sliced[limit] = true;
        int dist = 0;
        for (int curr = 0, prev = 0; curr <= limit; curr++)
            if (sliced[curr]) {
                dist = max(dist, curr - prev);
                prev = curr;
            }

        cout << fixed << setprecision(4) << M_PI * r * r * dist / limit << "\n";
    }
}
