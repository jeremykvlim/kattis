#include <bits/stdc++.h>
using namespace std;

int t(int i, vector<int> &times, vector<pair<int, int>> &groups) {
    if (!i) {
        groups.emplace_back(times[0], 0);
        return times[0];
    } else if (i == 1) {
        groups.emplace_back(times[0], times[1]);
        return times[1];
    } else if (i > 2 && 2 * times[1] < times[0] + times[i - 1]) {
        groups.emplace_back(times[0], times[1]);
        groups.emplace_back(times[0], 0);
        groups.emplace_back(times[i - 1], times[i]);
        groups.emplace_back(times[1], 0);
        return times[0] + 2 * times[1] + times[i] + t(i - 2, times, groups);
    } else {
        groups.emplace_back(times[0], times[i]);
        groups.emplace_back(times[0], 0);
        return times[0] + times[i] + t(i - 1, times, groups);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> times(n);
    for (int &t : times) cin >> t;
    sort(times.begin(), times.end());

    vector<pair<int ,int>> groups;
    cout << t(n - 1, times, groups) << "\n";
    for (auto [p1, p2] : groups) {
        if (p2) cout << p1 << " " << p2 << "\n";
        else cout << p1 << "\n";
    }
}
