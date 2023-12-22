#include <bits/stdc++.h>
using namespace std;

constexpr int LIMIT = 360 * 60 * 60;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int r, n, a, m, s;
        cin >> r >> n >> a >> m >> s;

        vector<bool> sliced(LIMIT + 1);
        for (int i = 0, pos = 0; i < n && !sliced[pos]; i++) {
            sliced[pos] = true;
            pos += a * 60 * 60 + m * 60 + s;
            if (pos >= LIMIT) pos -= LIMIT;
        }

        sliced[LIMIT] = true;
        int dist = 0;
        for (int curr = 0, prev = 0; curr <= LIMIT; curr++)
            if (sliced[curr]) {
                dist = max(dist, curr - prev);
                prev = curr;
            }

        cout << fixed << setprecision(4) << M_PI * r * r * dist / LIMIT << "\n";
    }
}
