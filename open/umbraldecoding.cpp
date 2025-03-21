#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, K;
    cin >> n >> K;

    vector<array<int, 3>> intervals;
    while (K--) {
        int x, y, b;
        cin >> x >> y >> b;

        for (int i = 1; i * i * i <= b; i++) {
            int l = 0, r = 1e3, m;
            while (l + 1 < r) {
                m = l + (r - l) / 2;

                if (m * m * m <= b - i * i * i) l = m;
                else r = m;
            }
            if (x <= n - i) intervals.push_back({x + i, max(0, y - l), min(n, y + l)});
            if (x >= i) intervals.push_back({x - i, max(0, y - l), min(n, y + l)});
        }
        int l = 0, r = 1e3, m;
        while (l + 1 < r) {
            m = l + (r - l) / 2;

            if (m * m * m <= b) l = m;
            else r = m;
        }
        intervals.push_back({x, max(0, y - l), min(n, y + l)});
    }
    sort(intervals.begin(), intervals.end());

    auto points = (long long) (n + 1) * (n + 1);
    for (int i = 0, j = i; i < intervals.size(); i = j) {
        auto [x1, yl1, yr1] = intervals[i];
        while (j < intervals.size() && x1 == intervals[j][0]) j++;

        for (int k = i; k < j; k++) {
            auto [x2, yl2, yr2] = intervals[k];
            if (yl2 > yr1) {
                points -= yr1 - yl1 + 1;
                yl1 = yl2;
                yr1 = yr2;
            } else yr1 = max(yr1, yr2);
        }
        points -= yr1 - yl1 + 1;
    }
    cout << points;
}
