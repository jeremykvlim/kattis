#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> kodamas(n);
    for (auto &[a, c] : kodamas) cin >> a >> c;
    sort(kodamas.begin(), kodamas.end(), [&](auto p1, auto p2) {return p1.second != p2.second ? p1.second > p2.second : p1.first < p2.first;});
    kodamas.erase(unique(kodamas.begin(), kodamas.end()), kodamas.end());

    n = kodamas.size();
    vector<int> compress(n);
    for (int i = 0; i < n; i++) compress[i] = kodamas[i].first;
    sort(compress.begin(), compress.end());
    compress.erase(unique(compress.begin(), compress.end()), compress.end());
    for (auto &[a, c] : kodamas) a = lower_bound(compress.begin(), compress.end(), a) - compress.begin();

    vector<vector<int>> adj_list(n);
    for (int i = 0; i < n; i++) {
        auto [a, c] = kodamas[i];
        adj_list[a].emplace_back(i);
    }

    int size = ceil(sqrt(n));
    vector<bool> valid(n, false);
    vector<int> dp(n, 0), seen(size, -1);
    vector<vector<int>> pref_max_a(size, vector<int>(size, -1)), pref_min_c(size, vector<int>(size, INT_MAX)), suff_min_dp(size, vector<int>(size, INT_MAX));
    auto query = [&](int c = INT_MAX) {
        auto it = find_if(pref_min_c.begin(), pref_min_c.end(), [c](const auto &block) {return block.back() < c;});
        if (it == pref_min_c.end()) return INT_MAX;

        int l = -1;
        for (int i = (it - pref_min_c.begin()) * size; i < n; i++)
            if (valid[i] && kodamas[i].second < c) {
                l = i;
                break;
            }
        if (l == -1) return INT_MAX;

        int v = dp[l] + 1, b = l / size;
        for (int i = l + 1; i < min(n, (b + 1) * size); i++)
            if (valid[i] && kodamas[l] < kodamas[i]) {
                v = min(v, dp[i] + 1);
                l = i;
            }

        for (int i = b + 1; i < size; i++) {
            int j = lower_bound(pref_max_a[i].begin(), pref_max_a[i].end(), kodamas[l].first) - pref_max_a[i].begin();
            if (j < size) v = min(v, suff_min_dp[i][j]);
            if (seen[i] != -1 && kodamas[l] <= kodamas[i * size + seen[i]]) l = i * size + seen[i];
        }
        return v;
    };

    for (auto indices : adj_list) {
        if (indices.empty()) continue;

        queue<pair<int, int>> q;
        sort(indices.begin(), indices.end());
        for (int i : indices) {
            int v = query(kodamas[i].second);
            q.emplace(i, v == INT_MAX ? 1 : v);
        }

        while (!q.empty()) {
            auto [l, v] = q.front();
            q.pop();

            dp[l] = v;
            valid[l] = true;

            int b = l / size, j = b * size, a = -1, c = INT_MAX;
            pair<int, int> k{-1, INT_MAX};
            vector<bool> visited(size, false);
            for (int i = 0; i < size; i++) {
                if (i + j < n && valid[i + j]) {
                    a = max(a, kodamas[i + j].first);
                    c = min(c, kodamas[i + j].second);

                    if (k < kodamas[i + j]) {
                        k = kodamas[i + j];
                        seen[b] = i;
                        visited[i] = true;
                    }
                }
                pref_max_a[b][i] = a;
                pref_min_c[b][i] = c;
            }

            v = INT_MAX;
            for (int i = size - 1; ~i; i--) {
                if (i + j < n && visited[i]) v = min(v, dp[i + j] + 1);
                suff_min_dp[b][i] = v;
            }
        }
    }
    cout << query() - 1;
}
