#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> endpoints(2 * n);
    vector<pair<int, int>> intervals(n);
    for (int i = 0; i < n; i++) {
        auto &[a, b] = intervals[i];
        cin >> a >> b;

        endpoints[2 * i] = a;
        endpoints[2 * i + 1] = b;
    }
    sort(intervals.begin(), intervals.end());
    sort(endpoints.begin(), endpoints.end());
    endpoints.erase(unique(endpoints.begin(), endpoints.end()), endpoints.end());

    vector<pair<int, int>> merged;
    for (auto [l, r] : intervals)
        if (merged.empty() || l > merged.back().second) merged.emplace_back(l, r);
        else merged.back().second = max(merged.back().second, r);

    for (auto [l, r] : merged) {
        for (int i = upper_bound(endpoints.begin(), endpoints.end(), l) - endpoints.begin(), j = lower_bound(endpoints.begin(), endpoints.end(), r) - endpoints.begin(); i < j; l = endpoints[i++]) cout << l << " " << endpoints[i] << "\n";
        cout << l << " " << r << "\n";
    }
}
