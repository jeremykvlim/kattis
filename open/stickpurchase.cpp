#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> ends(n, 0);
    while (m--) {
        int x, y;
        cin >> x >> y;

        ends[x] = max(ends[x], y);
    }

    auto base = 0LL;
    vector<vector<int>> adj_list(n + 1);
    list<int> active;
    vector<list<int>::iterator> its(n);
    for (int x = 0, y = 0, prev = -1; x < n; x++) {
        y = max(y, ends[x]);
        int len = max(0, y - x);
        if (!len) continue;

        base += (long long) (x - prev) * (n - x + 1);
        adj_list[len].emplace_back(x);
        its[x] = active.emplace(active.end(), x);
        prev = x;
    }

    auto sum = 0LL;
    for (int len = 1; len <= n; len++) {
        if (!active.empty()) sum += base - 1LL * len * (active.back() + 1);
        for (int x : adj_list[len]) {
            auto it_l = its[x];
            int l = it_l == active.begin() ? -1 : *prev(it_l);
            auto it_r = next(it_l);
            int r = it_r == active.end() ? -1 : *it_r;

            base -= (long long) (x - l) * (~r ? r - x : n - x + 1);
            active.erase(it_l);
        }
    }
    cout << fixed << setprecision(6) << (double) sum / ((long long) n * (n + 1) / 2);
}
