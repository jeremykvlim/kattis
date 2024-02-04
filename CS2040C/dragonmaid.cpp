#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<tuple<int, int, int>> items(n);
    for (int i = 0; i < n; i++) {
        int p, v;
        cin >> p >> v;

        items[i] = {v, p, i};
    }
    sort(items.begin(), items.end());

    int q;
    cin >> q;

    vector<tuple<int, int, int>> queries(q);
    for (int i = 0; i < q; i++) {
        int x, k;
        cin >> x >> k;

        queries[i] = {x, k, i};
    }
    sort(queries.begin(), queries.end());

    vector<vector<int>> indices(q);
    set<pair<int, int>> order;
    int m = 0;
    for (auto [x, k, i] : queries) {
        for (; m < n; m++) {
            auto [v, p, j] = items[m];
            if (v <= x) order.emplace(-p, j);
            else break;
        }

        for (auto it = order.begin(); k-- && it != order.end(); it++) indices[i].emplace_back(it->second);
    }

    for (auto &i : indices) {
        if (i.empty()) cout << "-1";
        else for (auto j : i) cout << j + 1 << " ";
        cout << "\n";
    }
}
