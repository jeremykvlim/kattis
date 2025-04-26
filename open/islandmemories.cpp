#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> all(m);
    for (int i = 0; i < m; i++) {
        int k;
        cin >> k;

        vector<int> islands(k);
        for (int &x : islands) {
            cin >> x;
            x--;
        }
        sort(islands.begin(), islands.end());
        islands.erase(unique(islands.begin(), islands.end()), islands.end());
        if (islands[0]) all[i] = islands;
        else
            for (int j = 0, x = 0; x < n; x++) {
                if (j < islands.size() && islands[j] == x) {
                    j++;
                    continue;
                }
                all[i].emplace_back(x);
            }
    }

    vector<int> indices(m);
    iota(indices.begin(), indices.end(), 0);
    sort(indices.begin(), indices.end(), [&](int i, int j) { return all[i].size() < all[j].size(); });

    vector<int> seen1(n, -1), seen2(n, -1);
    for (int i : indices) {
        int count = 0, total = 0;
        for (int x : all[i]) {
            if (seen1[x] == i || !~seen2[x]) continue;

            int j = seen2[x];
            count++;
            total += all[j].size();
            for (int y : all[j]) {
                if (!binary_search(all[i].begin(), all[i].end(), y)) {
                    cout << 0;
                    exit(0);
                }
                seen1[y] = i;
            }
        }

        if (count > 1 && total == all[i].size()) {
            cout << 0;
            exit(0);
        }

        for (int x : all[i]) seen2[x] = i;
    }
    cout << 1;
}
