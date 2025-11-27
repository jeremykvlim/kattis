#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, w, h;
    cin >> n >> w >> h;

    int perimeter = 2 * (h + w);
    vector<pair<int, int>> intervals(n);
    for (auto &i : intervals) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;

        auto convert = [&](int x, int y) {
            if (!x || y == h) return x + y;
            if (x == w) return 2 * h + w - y;
            return perimeter - x;
        };
        i = minmax(convert(x1, y1), convert(x2, y2));
    }
    sort(intervals.begin(), intervals.end());

    vector<int> pref_max(n, intervals[0].second), suff_min(n, perimeter);
    for (int i = 1; i < n; i++) pref_max[i] = max(pref_max[i - 1], intervals[i].second);
    for (int i = n - 2; ~i; i--) suff_min[i] = min(suff_min[i + 1], intervals[i + 1].second);

    for (int i = 0, prev = perimeter; i < n; i++) {
        if (prev > intervals[i].first && suff_min[i] > max(intervals[n - 1].first, pref_max[i])) {
            auto print = [&](double y) {
                if (y <= h) cout << fixed << setprecision(6) << "0 " << y;
                else if (y <= h + w) cout << fixed << setprecision(6) << y - h << " " << h;
                else if (y <= 2 * h + w) cout << fixed << setprecision(6) << w << " " << 2 * h + w - y;
                else cout << fixed << setprecision(6) << perimeter - y << " 0";
            };

            cout << "1\n";
            print(intervals[i].first + .5);
            cout << " ";
            print(suff_min[i] - .5);
            exit(0);
        }
        prev = min(prev, intervals[i].second);
    }
    cout << fixed << setprecision(6) << "2\n0 0.5 " << w << " " << h - 0.5 << "\n0.5 " << h << " " << w - 0.5 << " 0";
}
