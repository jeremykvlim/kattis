#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, c;
    long long K, L;
    cin >> n >> c >> K >> L;

    vector<int> P(c);
    vector<bool> chore(n, false);
    for (int i = 0; i < c; i++) {
        cin >> P[i];

        chore[--P[i]] = true;
    }

    vector<vector<int>> adj_list(n);
    for (int _ = 0; _ < n - 1; _++) {
        int a, b;
        cin >> a >> b;

        adj_list[a - 1].emplace_back(b - 1);
        adj_list[b - 1].emplace_back(a - 1);
    }

    vector<vector<int>> children(n);
    vector<int> depth(n, -1);
    depth[0] = 0;
    queue<int> q1;
    q1.emplace(0);
    while (!q1.empty()) {
        int v = q1.front();
        q1.pop();

        for (int u : adj_list[v])
            if (depth[u] == -1) {
                depth[u] = depth[v] + 1;
                q1.emplace(u);
                children[v].emplace_back(u);
            }
    }

    int max_d = *max_element(depth.begin(), depth.end());
    vector<vector<int>> level(max_d + 1);
    for (int v = 0; v < n; v++) level[depth[v]].emplace_back(v);

    vector<pair<int, int>> order;
    for (int d = max_d; ~d; d--)
        for (int v : level[d])
            for (int i = 0; i <= children[v].size(); i++) order.emplace_back(v, i);
    int m = order.size();

    vector<int> size(m, 0);
    vector<vector<int>> indices(n);
    for (int j = 0; j < m; j++) {
        auto [v, i] = order[j];

        if (i) {
            int x = indices[v][i - 1], y = indices[children[v][i - 1]].back();
            size[j] = size[x] + size[y] + 1;
        }
        indices[v].emplace_back(j);
    }

    vector<bool> chore_subtree(m);
    for (int j = 0; j < m; j++) {
        auto [v, i] = order[j];

        if (i) {
            int x = indices[v][i - 1], y = indices[children[v][i - 1]].back();
            chore_subtree[j] = chore_subtree[x] || chore_subtree[y];
        } else chore_subtree[j] = chore[v];
    }

    vector<int> count(m, 0);
    for (int j = 0; j < m; j++) {
        auto [v, i] = order[j];

        if (i && chore_subtree[j]) {
            int x = indices[v][i - 1], y = indices[children[v][i - 1]].back();
            count[j] = count[x];
            if (chore_subtree[y]) count[j] += count[y] + 1;
        }
    }

    vector<vector<int>> dp1(m), dp2(m), dp3(m);
    for (int j = 0; j < m; j++) {
        auto [v, i] = order[j];

        int s = size[j];
        dp1[j].assign(s + 1, 0);
        dp2[j].assign(s + 1, -3);
        dp3[j].assign(s + 1, 0);

        if (!chore_subtree[j] || !i) continue;

        int x = indices[v][i - 1], y = indices[children[v][i - 1]].back();
        if (!chore_subtree[y]) {
            for (int k = 0; k <= s; k++) dp1[j][k] = dp1[x][min(k, size[x])];
            continue;
        }

        vector<int> left(s + 1), right(s + 1);
        for (int k = 0; k <= s; k++) {
            left[k] = dp1[x][min(k, size[x])];
            right[k] = dp1[y][min(k, size[y])];
        }

        for (int k = 0; k <= s; k++) {
            dp1[j][k] = left[k] + count[y] + 1;
            dp2[j][k] = -2;

            if (k - count[y] >= 1)
                if (dp1[j][k] > left[k - count[y] - 1]) {
                    dp1[j][k] = left[k - count[y] - 1];
                    dp2[j][k] = -1;
                }

            for (int l = max(0, k - size[y] - 1); l <= min(k - 1, size[x]); l++) {
                int dist = left[l] + right[k - l - 1] + 1;
                if (dp1[j][k] > dist) {
                    dp1[j][k] = dist;
                    dp2[j][k] = l;
                    dp3[j][k] = k - l - 1;
                }
            }
        }
    }

    auto time = LLONG_MAX;
    int l = 0;
    for (int k = 0; k <= size[m - 1]; k++) {
        auto t = max(K * dp1[m - 1][k], L * k);
        if (time > t) {
            time = t;
            l = k;
        }
    }

    vector<bool> fluttershy(n, false);
    fluttershy[0] = true;
    auto dfs = [&](auto &&self, int j, int k, bool split = false) -> void {
        auto [v, i] = order[j];

        if (!i) return;

        int x = indices[v][i - 1], y = indices[children[v][i - 1]].back();
        if (split) {
            if (chore_subtree[y]) fluttershy[children[v][i - 1]] = k == -1;
            self(self, x, k, true);
            self(self, y, k, true);
            return;
        }

        if (dp2[j][k] >= 0) {
            self(self, x, dp2[j][k]);
            fluttershy[children[v][i - 1]] = true;
            self(self, y, dp3[j][k]);
        } else if (dp2[j][k] == -1) {
            self(self, x, max(0, min(k - count[y] - 1, size[x])));
            fluttershy[children[v][i - 1]] = true;
            self(self, y, dp2[j][k], true);
        } else if (dp2[j][k] == -2) {
            self(self, x, min(k, size[x]));
            self(self, y, dp2[j][k], true);
        } else self(self, x, min(k, size[x]));
    };
    dfs(dfs, m - 1, l);

    vector<int> p, q;
    for (int i = 0; i < c; i++) (fluttershy[P[i]] ? p : q).emplace_back(i);

    auto transfer = [&](auto &v1, auto &v2) {
        auto it = min_element(v1.begin(), v1.end(), [&](int i, int j) { return depth[P[i]] < depth[P[j]]; });
        v2.emplace_back(*it);
        v1.erase(it);
    };
    if (q.empty()) transfer(p, q);
    else if (p.empty()) transfer(q, p);

    cout << p.size() << " " << q.size() << "\n";
    for (int pi : p) cout << pi + 1 << " ";
    cout << "\n";
    for (int qi : q) cout << qi + 1 << " ";
}
