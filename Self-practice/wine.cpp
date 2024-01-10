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
        set<pair<int, int>> pq;
        pq.emplace(0, 0);
        while (!pq.empty()) {
            auto curr = *pq.begin();
            pq.erase(pq.begin());

            for (auto [minimum, maximum] : bottles) {
                auto next = make_pair(curr.first + minimum, curr.second + maximum);
                if (next.first > wine) continue;
                if (next.second >= wine) {
                    cout << "0\n";
                    goto next;
                }

                auto it = pq.lower_bound(next);
                while (it != pq.begin() && prev(it)->second >= next.first) {
                    next = {min(prev(it)->first, next.first), max(prev(it)->second, next.second)};
                    it = pq.erase(prev(it));
                }

                it = pq.lower_bound(next);
                while (it != pq.end() && it->first <= next.second) {
                    next = {min(it->first, next.first), max(it->second, next.second)};
                    it = pq.erase(it);
                }

                if (curr != next) {
                    bottled = max(bottled, next.second);
                    pq.emplace(next);
                }
            }
        }

        cout << wine - bottled << "\n";
        next:;
    }
}
