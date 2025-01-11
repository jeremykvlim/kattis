#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, m;
    cin >> n >> k >> m;

    vector<int> arr(n + k + 1);
    vector<set<int>> indices(k + 1);
    for (int i = 1; i <= n + k; i++) {
        if (i <= n) cin >> arr[i];
        else arr[i] = i - n;

        indices[arr[i]].emplace(i);
    }

    set<array<int, 3>> intervals;
    multiset<int> lengths;
    auto add = [&](int l, int r, int i) {
        intervals.insert({l, r, i});
        if (i <= n) lengths.emplace(i - r);
    };

    auto remove = [&](int r, int i, auto it) {
        intervals.erase(it);
        if (i <= n) lengths.erase(lengths.lower_bound(i - r));
    };

    auto update = [&](int l1, int r1, int i) {
        if (l1 > r1) return;

        int temp = l1;
        while (l1 <= r1) {
            auto it = intervals.lower_bound({l1 + 1, (int) -1e9, (int) -1e9});
            auto [l2, r2, j] = *(--it);
            remove(r2, j, it);
            if (l2 < l1) add(l2, l1 - 1, j);
            if (r1 < r2) add(r1 + 1, r2, j);
            l1 = r2 + 1;
        }
        l1 = temp;

        auto it1 = intervals.lower_bound({l1, (int) 1e9, (int) 1e9});
        if (it1 != intervals.begin()) {
            auto [l2, r2, j] = *(--it1);
            if (i == j) {
                l1 = l2;
                remove(r2, j, it1);
            }
        }
        auto it2 = intervals.lower_bound({r1 + 1, (int) -1e9, (int) -1e9});
        if (it2 != intervals.end()) {
            auto [l2, r2, j] = *it2;
            if (i == j) {
                r1 = r2;
                remove(r2, j, it2);
            }
        }
        add(l1, r1, i);
    };

    add(1, n + k, 1e9);
    vector<int> count(k + 1, 0);
    for (int i = 1, r = 1, unique = 0; i <= n + k; i++) {
        if (++count[arr[i]] == 1) unique++;

        int l = r;
        while (r <= n + k) {
            if (unique != k) break;
            if (!--count[arr[r]]) unique--;
            r++;
        }
        if (l < r) update(l, r - 1, i);
    }

    while (m--) {
        int q;
        cin >> q;

        if (q == 1) {
            int p, v;
            cin >> p >> v;

            if (arr[p] == v) continue;

            int l1 = 1, r1 = p;
            auto it = indices[arr[p]].lower_bound(p);
            if (it != indices[arr[p]].begin()) l1 = *prev(it) + 1;
            indices[arr[p]].erase(p);

            int j = 1e9;
            it = indices[arr[p]].lower_bound(p + 1);
            if (it != indices[arr[p]].end()) j = *it;

            auto pos = [&](int p) {
                auto [l, r, i] = *prev(intervals.lower_bound({p + 1, (int) -1e9, (int) -1e9}));
                return i;
            };

            int l = l1 - 1, r = r1 + 1, mid;
            while (l + 1 < r) {
                mid = l + (r - l) / 2;

                if (pos(mid) <= j) l = mid;
                else r = mid;
            }
            update(l1, l, j);

            arr[p] = v;
            indices[v].emplace(p);

            j = 1e9;
            it = indices[v].lower_bound(p + 1);
            if (it != indices[v].end()) j = *it;

            l = 0, r = p + 1;
            while (l + 1 < r) {
                mid = l + (r - l) / 2;

                if (pos(mid) >= j) r = mid;
                else l = mid;
            }

            if (r == j) continue;
            auto [l2, r2, i] = *intervals.lower_bound({r, (int) -1e9, (int) -1e9});
            if (i != j) continue;

            l1 = l2;
            r1 = min(p, r2);
            vector<pair<int, int>> valid;
            for (int i = 1, temp = l1; i <= k; i++, l1 = temp) {
                it = indices[i].lower_bound(p);
                if (it != indices[i].end() && *it < j) {
                    if (it != indices[i].begin()) l1 = max(l1, *prev(it) + 1);
                    if (l1 <= r1) valid.emplace_back(*it, l1);
                }
            }
            sort(valid.begin(), valid.end());
            for (auto [i, l1] : valid) update(l1, r1, i);
        } else {
            if (lengths.empty()) cout << "-1\n";
            else cout << *lengths.begin() + 1 << "\n";
        }
    }
}
