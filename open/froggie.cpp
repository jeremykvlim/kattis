#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int l, w;
    cin >> l >> w;

    vector<array<int, 3>> cars(l);
    for (auto &[o, i, s] : cars) cin >> o >> i >> s;

    int p;
    string s;
    cin >> p >> s;

    int y = l;
    bool entered = false;
    unordered_map<char, int> dtoi{{'R', 0}, {'U', 1}, {'L', 2}, {'D', 3}};
    vector<int> dx{1, 0, -1, 0}, dy{0, 1, 0, -1};
    for (int i = 0; i < s.size(); i++) {
        char c = s[i];

        p += dx[dtoi[c]];
        y -= dy[dtoi[c]];
        if (y >= l && entered || !(0 <= p && p < w)) {
            cout << "squish";
            exit(0);
        }
        if (!(0 <= y && y < l)) continue;

        entered = true;
        int k = (cars[y][0] + (i + 1) * cars[y][2]) % cars[y][1];
        vector<bool> hit(cars[y][1], false);
        hit[k] = true;
        for (int j = 1; j < cars[y][2]; j++) {
            k = (k + cars[y][1] - 1) % cars[y][1];
            hit[k] = true;
        }

        if (hit[(y & 1 ? w - p - 1 : p) % cars[y][1]]) {
            cout << "squish";
            exit(0);
        }
    }
    cout << (y < 0 ? "safe" : "squish");;
}
