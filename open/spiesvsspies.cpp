#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> x(n + 1), y(n + 1);
    for (int i = 1; i <= n; i++) cin >> x[i] >> y[i];

    unordered_map<int, vector<pair<int, int>>> rows, cols;
    for (int i = 1; i <= n; i++) {
        rows[y[i]].emplace_back(x[i], i);
        cols[x[i]].emplace_back(y[i], i);
    }

    vector<int> u(n + 1, 0), d(n + 1, 0), l(n + 1, 0), r(n + 1, 0);
    for (auto &[ys, xs] : rows) {
        sort(xs.begin(), xs.end());
        for (int k = 0; k < xs.size(); k++) {
            int i = xs[k].second;
            if (k) l[i] = xs[k - 1].second;
            if (k + 1 < xs.size()) r[i] = xs[k + 1].second;
        }
    }
    for (auto &[xs, ys] : cols) {
        sort(ys.begin(), ys.end());
        for (int k = 0; k < ys.size(); k++) {
            int i = ys[k].second;
            if (k) d[i] = ys[k - 1].second;
            if (k + 1 < ys.size()) u[i] = ys[k + 1].second;
        }
    }

    vector<bool> eliminated(n + 1, false);
    while (m--) {
        int t;
        char c;
        cin >> t >> c;

        if (eliminated[t]) {
            cout << "ignore\n";
            continue;
        }

        auto eliminate = [&](int i) {
            eliminated[i] = true;
            int up = u[i], down = d[i], left = l[i], right = r[i];
            if (up) d[up] = down;
            if (down) u[down] = up;
            if (left) r[left] = right;
            if (right) l[right] = left;
            l[i] = r[i] = u[i] = d[i] = 0;
        };

        vector<int> spies;
        auto walk = [&](const auto &dir) {
            for (int i = dir[t], j = dir[i]; i; j = dir[i = j]) {
                spies.emplace_back(i);
                eliminate(i);
            }
        };
        if (c == 'N') walk(u);
        else if (c == 'S') walk(d);
        else if (c == 'E') walk(r);
        else walk(l);

        cout << spies.size() << " ";
        for (int i : spies) cout << i << " ";
        cout << "\n";
    }
}
