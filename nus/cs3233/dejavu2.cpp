#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, l;
    cin >> n >> k >> l;

    vector<int> h(n);
    for (int &hi : h) cin >> hi;
    sort(h.rbegin(), h.rend());

    vector<long long> pref(n + 1, 0), diff(n + 1);
    for (int i = 0; i < n; i++) {
        pref[i + 1] = pref[i] + h[i];
        if (i < n - 1) diff[i + 1] = h[i + 1] - h[i];
    }

    auto pref_sum = [&](int l, int r) {
        if (l > r) return 0LL;
        return pref[r + 1] - pref[l];
    };

    priority_queue<tuple<long long, pair<int, int>, pair<int, int>>> pq;
    pq.emplace(pref_sum(0, k - 1), make_pair(0, k - 1), make_pair(0, -1));
    while (l--) {
        if (pq.empty()) {
            cout << "-1\n";
            continue;
        }

        auto [sum, seg1, seg2] = pq.top();
        pq.pop();

        cout << sum << "\n";

        auto process = [&](auto seg, int m, bool first) {
            auto [l1, r1] = seg;
            if (r1 + 1 < m) {
                queue<pair<int, int>> q;
                if (l1 <= r1 - 1) q.emplace(l1, r1 - 1);
                q.emplace(r1 + 1, r1 + 1);
                if (first && seg2 != make_pair(0, -1)) q.emplace(seg2);

                vector<pair<int, int>> next(3, {0, -1});
                next[0] = q.front();
                q.pop();

                int i = 1;
                while (!q.empty()) {
                    auto [l2, r2] = q.front();
                    q.pop();

                    if (next[i - 1].second + 1 < l2) next[i++] = {l2, r2};
                    else next[i - 1].second = max(next[i - 1].second, r2);
                }
                pq.emplace(sum + diff[r1 + 1], next[0], next[1]);
            }
        };
        process(seg1, (seg2 != make_pair(0, -1) ? seg2.first : n), true);
        if (seg2 != make_pair(0, -1)) process(seg2, n, false);
    }
}
