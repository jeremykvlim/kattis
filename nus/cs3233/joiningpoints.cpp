#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<long long, long long>> points(n);
    long long xl = 1e18, xr = -1e18, yl = 1e18, yr = -1e18;
    for (auto &[x, y] : points) {
        cin >> x >> y;

        xl = min(xl, x);
        xr = max(xr, x);
        yl = min(yl, y);
        yr = max(yr, y);
    }

    auto width = xr - xl, height = yr - yl, square_len = max(width, height);
    vector<array<long long, 4>> candidates;
    if (width == height) candidates.push_back({xl, xr, yl, yr});
    else if (width > height) {
        candidates.push_back({xl, xr, yl, yl + square_len});
        candidates.push_back({xl, xr, yr - square_len, yr});
    } else {
        candidates.push_back({xl, xl + square_len, yl, yr});
        candidates.push_back({xr - square_len, xr, yl, yr});
    }

    int s = candidates.size();
    vector<bool> possible(s, true);
    for (auto [x, y] : points) {
        for (int i = 0; i < s; i++) {
            if (!possible[i]) continue;
            auto [x1, x2, y1, y2] = candidates[i];
            if (!(x == x1 || x == x2 || y == y1 || y == y2)) possible[i] = false;
        }

        if (all_of(possible.begin(), possible.end(), [&](bool b) { return !b; })) {
            cout << "Impossible";
            exit(0);
        }
    }

    for (int i = 0; i < s; i++)
        if (possible[i]) {
            auto [x1, x2, y1, y2] = candidates[i];
            cout << x1 << " " << x2 << " " << y1 << " " << y2;
            exit(0);
        }
}
