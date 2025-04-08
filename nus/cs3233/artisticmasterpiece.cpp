#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, r;
    string s;
    cin >> n >> r >> s;

    vector<pair<int, int>> offsets;
    for (int dy = 0;; dy++) {
        int dx = r - dy * dy * dy * dy;
        if (dx < 0) break;

        offsets.emplace_back(dy, floor(sqrt(dx)));
    }

    unordered_map<int, vector<pair<int, int>>> intervals;
    auto add = [&](int x, int y) {
        for (auto [dy, dx] : offsets) {
            intervals[y + dy].emplace_back(x - dx, x + dx + 1);
            intervals[y - dy].emplace_back(x - dx, x + dx + 1);
        }
    };

    int x = 0, y = 0;
    add(x, y);
    for (char c : s) {
        switch (c) {
            case 'L': x--; break;
            case 'R': x++; break;
            case 'U': y++; break;
            case 'D': y--; break;
        }
        add(x, y);
    }

    auto pixels = 0LL;
    for (auto [_, itvls] : intervals) {
        sort(itvls.begin(), itvls.end());

        int prev = INT_MIN;
        for (auto [l, r] : itvls) {
            prev = max(prev, l);
            if (r > prev) {
                pixels += r - prev;
                prev = r;
            }
        }
    }

    cout << pixels;
}
