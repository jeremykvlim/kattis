#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int p, n;
    cin >> p >> n;

    vector<string> c(p);
    vector<pair<int, int>> intervals(p);
    for (int i = 0; i < p; i++) {
        cin >> c[i];

        if (i + 1 < p) cin >> intervals[i + 1].first >> intervals[i + 1].second;
    }

    vector<set<int>> times(p);
    while (n--) {
        string v;
        int t;
        cin >> v >> t;

        int i = find(c.begin(), c.end(), v) - c.begin();
        if (i == p) continue;

        if (!i || times[i - 1].lower_bound(t - intervals[i].second) != times[i - 1].upper_bound(t - intervals[i].first)) times[i].emplace(t);
    }
    cout << (!times[p - 1].empty() ? "Yes" : "No");
}
