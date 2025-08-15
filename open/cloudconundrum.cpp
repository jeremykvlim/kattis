#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    if (!k) {
        cout << 0;
        exit(0);
    }

    vector<pair<int, int>> workorders(n);
    for (auto &[s, l] : workorders) {
        cin >> s >> l;

        l += s;
    }
    sort(workorders.begin(), workorders.end());

    int l = 0, r = min(k, n), m;
    while (l + 1 < r) {
        m = l + (r - l) / 2;

        int count = 0;
        multiset<int> active;
        for (auto [tl, tr] : workorders) {
            while (!active.empty() && *active.begin() <= tl) active.erase(active.begin());

            active.emplace(tr);
            count++;

            if (active.size() > m) {
                auto it = prev(active.end());
                active.erase(it);
                count--;
            }
        }

        if (count >= k) r = m;
        else l = m;
    }
    cout << r;
}
