#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int x_max = 0;
    vector<int> v(n);
    vector<vector<int>> indices(15001);
    for (int i = 0; i < n; i++) {
        cin >> v[i];

        x_max = max(x_max, v[i]);
        indices[v[i]].emplace_back(i);
    }

    if (adjacent_find(v.begin(), v.end(), not_equal_to<>()) == v.end()) {
        cout << v[0];
        exit(0);
    }

    if (!(n & 1)) {
        cout << "none";
        exit(0);
    }

    int m = n / 2;
    for (int x = 1; x <= x_max; x++)
        if (!indices[x].empty()) {
            int diff = n + indices[x][0] - indices[x].back();
            for (int i = 0; i + 1 < indices[x].size(); i++) diff = max(diff, indices[x][i + 1] - indices[x][i]);
            if (n - diff + 1 > m) {
                cout << "none";
                exit(0);
            }
        }

    vector<bool> skip(x_max + 1, false);
    for (int i = 0; i < n; i++)
        if (v[i] != v[(i + 1) % n] && indices[v[(i + m + 1) % n]].size() > 1) skip[v[i]] = skip[v[(i + 1) % n]] = true;

    vector<int> count(n + 1, 0);
    auto add = [&](int l, int r) {
        if (l > r) return;
        count[l]++;
        if (r + 1 < n) count[r + 1]--;
    };
    for (int x = 1; x <= x_max; x++)
        if (indices[x].size() > 1) {
            int k = indices[x].size();
            vector<int> cycle_indices(2 * k);
            for (int i = 0; i < k; i++) {
                cycle_indices[i] = indices[x][i];
                cycle_indices[i + k] = indices[x][i] + n;
            }

            for (int l = 0, r = 0; l < cycle_indices.size() && cycle_indices[l] < n; r = max(r, ++l)) {
                for (; r + 1 < cycle_indices.size() && cycle_indices[r + 1] - cycle_indices[l] <= m; r++);
                if (l < r && cycle_indices[r] - cycle_indices[l] > r - l) {
                    if (cycle_indices[r] < n) add(cycle_indices[l], cycle_indices[r]);
                    else {
                        add(cycle_indices[l], n - 1);
                        add(0, cycle_indices[r] - n);
                    }
                }
            }
        }

    for (int i = 0, delta = 0; i < n; i++) {
        delta += count[i];
        if (delta > 0) skip[v[i]] = true;
    }

    vector<int> choose;
    for (int x = 1; x <= x_max; x++)
        if (!indices[x].empty() && !skip[x]) choose.emplace_back(x);

    if (choose.empty()) cout << "none";
    else
        for (int x : choose) cout << x << "\n";
}
