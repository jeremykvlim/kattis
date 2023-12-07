#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<tuple<int, int, char>> components(n);
    for (auto &c : components) {
        int x, y;
        char r;
        cin >> x >> y >> r;
        c = make_tuple(2 * x + y, -2 * x + y, r);
    }
    sort(components.begin(), components.end());

    int leak = 0, lowest = INT_MAX;
    for (auto &c : components) {
        int y = get<1>(c);
        char r = get<2>(c);
        if (y >= lowest) {
            if (r == 'P') leak = -n - 1;
            get<2>(c) = 'N';
        } else if (r == 'N') lowest = y;
    }

    for (int i = n - 1, curr = INT_MIN, prev = INT_MIN; i >= 0; i--) {
        int y = get<1>(components[i]);
        char r = get<2>(components[i]);
        if (r == 'N') continue;
        curr = max(curr, y);
        if (r == 'P' && y > prev) {
            leak++;
            prev = curr;
        }
    }

    cout << (leak < 0 ? "impossible" : to_string(leak));
}
