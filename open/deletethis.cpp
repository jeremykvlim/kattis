#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int nr, nc, n, m;
    cin >> nr >> nc >> n >> m;

    vector<pair<int, int>> files(n + m);
    vector<int> indices, rows{0, nr}, cols{0, nc};
    for (int i = 0; i < n + m; i++) {
        cin >> files[i].first >> files[i].second;

        files[i].first += floor(15 / 2);
        files[i].second += floor(9 / 2);

        if (files[i].first < nr && files[i].second < nc) {
            indices.emplace_back(i);
            rows.emplace_back(files[i].first);
            cols.emplace_back(files[i].second);
        }
    }
    sort(rows.begin(), rows.end());
    sort(cols.begin(), cols.end());
    rows.erase(unique(rows.begin(), rows.end()), rows.end());
    cols.erase(unique(cols.begin(), cols.end()), cols.end());

    for (int i : indices) {
        auto &[pr, pc] = files[i];
        pr = lower_bound(rows.begin(), rows.end(), pr) - rows.begin();
        pc = lower_bound(cols.begin(), cols.end(), pc) - cols.begin();
    }

    int del = n;
    for (int l = 0; l < rows.size(); l++)
        for (int r = l; r < rows.size(); r++) {
            vector<int> pref(cols.size(), 0);
            for (int i : indices)
                if (l <= files[i].first && files[i].first <= r) pref[files[i].second] += i < n ? -1 : 1;

            int most = 0;
            for (int i = 0; i < cols.size(); i++) {
                if (i) pref[i] += pref[i - 1];
                most = max(most, pref[i]);
                del = min(del, n + pref[i] - most);
            }
        }

    cout << del;
}
