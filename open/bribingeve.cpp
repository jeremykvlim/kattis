#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m1, m2;
    cin >> n >> m1 >> m2;

    int above = 0, below = 0;
    map<double, array<int, 2>> sweep;
    for (int _ = 0; _ < n - 1; _++) {
        int x1, x2;
        cin >> x1 >> x2;

        int dx1 = x1 - m1, dx2 = x2 - m2;

        if (dx1 > 0 && dx2 > 0) above++;
        else if (dx1 < 0 && dx2 < 0) below++;
        else {
            auto ratio = !dx1 ? -1e18 : (double) dx2 / dx1;
            if (dx1 < 0) {
                sweep[ratio][0]++;
                below++;
            } else if (dx1 > 0) {
                sweep[ratio][1]++;
                above++;
            } else if (dx2 < 0) {
                sweep[ratio][1]++;
                above++;
            } else if (dx2 > 0) {
                sweep[ratio][0]++;
                below++;
            }
        }
    }

    int a = above, b = below;
    for (auto [_, count] : sweep) {
        below -= count[0];
        above -= count[1];
        b = min(b, below);
        a = min(a, above);
        below += count[1];
        above += count[0];
    }
    cout << a + 1 << " " << n - b;
}
