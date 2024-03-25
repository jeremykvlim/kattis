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
        c = {2 * x + y, -2 * x + y, r};
    }
    sort(components.begin(), components.end());

    int leak = 0, lowest = INT_MAX;
    for (auto &[x, y, r] : components)
        if (lowest <= y) {
            if (r == 'P') leak = -n - 1;
            r = 'N';
        } else if (r == 'N') lowest = y;

    reverse(components.begin(), components.end());
    int curr = INT_MIN, prev = INT_MIN;
    for (auto [x, y, r] : components) {
        if (r == 'N') continue;

        curr = max(curr, y);
        if (r == 'P' && y > prev) {
            leak++;
            prev = curr;
        }
    }

    cout << (leak < 0 ? "impossible" : to_string(leak));
}
