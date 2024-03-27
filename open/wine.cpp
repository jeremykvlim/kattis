#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int wine, sizes;
        cin >> wine >> sizes;

        wine *= 1000;
        vector<pair<int, int>> bottles(sizes);
        int least = INT_MAX;
        for (auto &[minimum, maximum] : bottles) {
            cin >> minimum >> maximum;

            if (minimum != maximum) least = min(least, maximum * minimum / (maximum - minimum));
        }

        if (wine > least) {
            cout << "0\n";
            continue;
        }

        int bottled = 0;
        set<pair<int, int>> pq{{0, 0}};
        while (!pq.empty()) {
            auto [l1, r1] = *pq.begin();
            pq.erase(pq.begin());

            for (auto [minimum, maximum] : bottles) {
                auto [l2, r2] = make_pair(l1 + minimum, r1 + maximum);
                if (l2 > wine) continue;
                if (r2 >= wine) {
                    cout << "0\n";
                    goto next;
                }

                auto it = pq.lower_bound({l2, r2});
                while (it != pq.begin() && prev(it)->second >= l2) {
                    l2 = min(prev(it)->first, l2);
                    r2 = max(prev(it)->second, r2);
                    it = pq.erase(prev(it));
                }

                it = pq.lower_bound({l2, r2});
                while (it != pq.end() && it->first <= r2) {
                    l2 = min(it->first, l2);
                    r2 = max(it->second, r2);
                    it = pq.erase(it);
                }

                bottled = max(bottled, r2);
                pq.emplace(l2, r2);
            }
        }

        cout << wine - bottled << "\n";
        next:;
    }
}
