#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> intervals(n);
    for (auto &[a, b] : intervals) cin >> a >> b;
    sort(intervals.begin(), intervals.end(), [](auto p1, auto p2) { return p1.first != p2.first ? p1.first > p2.first : p1.second < p2.second; });

    vector<int> r(n), indices(n, -1), next(n, -1);
    auto process = [&](int i, int &j) {
        next[i] = indices[j++];
        r[j] = intervals[i].second;
        indices[j] = i;
    };

    int k = 0;
    for (int i = 0; i < n; i++) {
        int j = upper_bound(r.begin() + 1, r.begin() + k + 1, intervals[i].second) - r.begin() - 1;
        process(i, r[k] > intervals[i].second ? j : k);
    }

    cout << k << "\n";
    for (int i = indices[k]; ~i; i = next[i]) cout << intervals[i].first << " " << intervals[i].second << "\n";
}
