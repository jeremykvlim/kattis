#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    auto covered = 0LL;
    map<int, int> intervals;
    while (q--) {
        int t;
        cin >> t;

        if (t == 1) {
            int s, e;
            cin >> s >> e;

            int l = s, r = e;
            auto it = intervals.lower_bound(s);
            if (it != intervals.begin()) {
                auto temp = prev(it);
                if (temp->second >= s - 1) it = temp;
            }

            auto temp = it;
            for (; it != intervals.end() && it->first <= r + 1; it++) {
                l = min(l, it->first);
                r = max(r, it->second);
                covered -= it->second - it->first + 1;
            }
            if (temp != it) intervals.erase(temp, it);
            intervals[l] = r;
            covered += r - l + 1;
        } else cout << covered << "\n";
    }
}
