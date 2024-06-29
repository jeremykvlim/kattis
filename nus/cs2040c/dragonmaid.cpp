#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<array<int, 3>> items(n);
    for (int i = 0; i < n; i++) {
        int p, v;
        cin >> p >> v;

        items[i] = {v, p, i};
    }
    sort(items.begin(), items.end());

    int q;
    cin >> q;

    vector<array<int, 3>> queries(q);
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
            if (items[m][0] <= x) order.emplace(-items[m][1], items[m][2]);
            else break;
        }

        for (auto it = order.begin(); k && it != order.end(); k--, it++) indices[i].emplace_back(it->second);
    }

    for (auto &i : indices) {
        if (i.empty()) cout << "-1";
        else 
            for (int j : i) cout << j + 1 << " ";
        cout << "\n";
    }
}
