#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, l, w;
    cin >> n >> l >> w;

    vector<pair<int, int>> corners;
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;

        if (x % l) x = (x / l + 1) * l + 1;
        else x++;

        if (y % w) y = (y / w + 1) * w + 1;
        else y++;

        corners.emplace_back(x, y);
    }
    sort(corners.begin(), corners.end());
    corners.erase(unique(corners.begin(), corners.end()), corners.end());

    cout << corners.size() << "\n";
    for (auto [x, y] : corners) cout << x << " " << y << "\n";
}
