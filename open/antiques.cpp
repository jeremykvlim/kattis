#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<int> a(n + 1), p(n + 1), b(n + 1), q(n + 1), count(m + 1, 0);
    vector<vector<int>> knockoffs(m + 1);
    for (int i = 1; i <= n; i++) {
        cin >> a[i] >> p[i] >> b[i] >> q[i];

        if (a[i] > b[i]) {
            swap(a[i], b[i]);
            swap(p[i], q[i]);
        }

        knockoffs[b[i]].emplace_back(i);
        count[a[i]]++;
        count[b[i]]++;
    }

    vector<bool> visited(m + 1, false);
    int cost = INT_MAX;
    auto dfs = [&](auto &&self, int v = 1, int depth = 0) {
        if (depth > k) return;
        if (v > m) {
            int c = 0;
            for (int i = 1; i <= n; i++)
                if (visited[a[i]] && visited[b[i]]) c += min(p[i], q[i]);
                else c += visited[a[i]] ? p[i] : q[i];

            cost = min(cost, c);
            return;
        }

        if (!any_of(knockoffs[v].begin(), knockoffs[v].end(), [&](int u) { return !visited[a[u]]; })) self(self, v + 1, depth);

        if (count[v]) {
            visited[v] = true;
            self(self, v + 1, depth + 1);
            visited[v] = false;
        }
    };
    dfs(dfs);

    cout << (cost == INT_MAX ? -1 : cost);
}
