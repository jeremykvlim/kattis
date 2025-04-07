#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<pair<int, int>> times(n);
    for (auto &[y, x] : times) cin >> x >> y;
    sort(times.begin(), times.end());

    multiset<int> ms;
    int count = 0;
    for (auto [y, x] : times)
        if (ms.size() < k || *ms.begin() <= x) {
            if (ms.size() && *ms.begin() <= x) ms.erase(prev(ms.upper_bound(x)));
            ms.emplace(y);
            count++;
        }
    cout << count;
}
