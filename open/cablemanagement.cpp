#include <bits/stdc++.h>
using namespace std;

tuple<vector<int>, vector<int>, int> hopcroft_karp(int n, int m, const vector<pair<int, int>> &edges) {
    vector<int> adj_list(edges.size()), l(n, -1), r(m, -1), degree(n + 1, 0);
    for (auto [u, v] : edges) degree[u]++;
    for (int i = 1; i <= n; i++) degree[i] += degree[i - 1];
    for (auto [u, v] : edges) adj_list[--degree[u]] = v;

    int matches = 0;
    vector<int> src(n), prev(n);
    queue<int> q;
    for (;;) {
        fill(src.begin(), src.end(), -1);
        fill(prev.begin(), prev.end(), -1);

        for (int i = 0; i < n; i++)
            if (!~l[i]) q.emplace(src[i] = prev[i] = i);

        int temp = matches;
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            if (~l[src[v]]) continue;

            for (int j = degree[v]; j < degree[v + 1]; j++) {
                int u = adj_list[j];

                if (!~r[u]) {
                    while (~u) {
                        r[u] = v;
                        swap(l[v], u);
                        v = prev[v];
                    }

                    matches++;
                    break;
                }

                if (!~prev[r[u]]) {
                    q.emplace(u = r[u]);
                    prev[u] = v;
                    src[u] = src[v];
                }
            }
        }

        if (temp == matches) return {l, r, matches};
    }
}

pair<vector<int>, vector<int>> konig(int n, int m, const vector<vector<int>> &adj_list, const vector<int> &l, const vector<int> &r) {
    vector<bool> visited_l(n, false), visited_r(m, false);
    queue<pair<int, bool>> q;
    for (int v = 0; v < n; v++)
        if (!~l[v]) {
            visited_l[v] = true;
            q.emplace(v, true);
        }

    while (!q.empty()) {
        auto [v, left] = q.front();
        q.pop();

        if (left) {
            for (int u : adj_list[v])
                if (!visited_r[u] && l[v] != u) {
                    visited_r[u] = true;
                    q.emplace(u, false);
                }
        } else {
            int u = r[v];
            if (u != -1 && !visited_l[u]) {
                visited_l[u] = true;
                q.emplace(u, true);
            }
        }
    }

    vector<int> cover_l, cover_r;
    for (int v = 0; v < n; v++)
        if (!visited_l[v]) cover_l.emplace_back(v);
    for (int v = 0; v < m; v++)
        if (visited_r[v]) cover_r.emplace_back(v);

    return {cover_l, cover_r};
}

tuple<int, vector<int>, vector<vector<int>>> tarjan(int n, vector<vector<int>> &adj_list) {
    vector<int> order(n, 0), low(n, 0), component(n, 0);
    vector<vector<int>> members(1);
    vector<bool> stacked(n, false);
    stack<int> st;
    int count = 0, sccs = 0;

    auto dfs = [&](auto &&self, int v) -> void {
        order[v] = low[v] = ++count;
        st.emplace(v);
        stacked[v] = true;

        for (int u : adj_list[v])
            if (!order[u]) {
                self(self, u);
                low[v] = min(low[v], low[u]);
            } else if (stacked[u]) low[v] = min(low[v], order[u]);

        if (order[v] == low[v]) {
            sccs++;
            members.emplace_back();
            int u;
            do {
                u = st.top();
                st.pop();
                stacked[u] = false;
                component[u] = sccs;
                members[sccs].emplace_back(u);
            } while (u != v);
        }
    };

    for (int v = 0; v < n; v++)
        if (!order[v]) dfs(dfs, v);

    return {sccs, component, members};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int N;
        cin >> N;

        vector<pair<int, int>> points(N);
        vector<int> xs(N), ys(N);
        for (int i = 0; i < N; i++) {
            auto &[x, y] = points[i];
            cin >> x >> y;

            xs[i] = x;
            ys[i] = y;
        }
        sort(xs.begin(), xs.end());
        sort(ys.begin(), ys.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
        ys.erase(unique(ys.begin(), ys.end()), ys.end());

        int n = xs.size(), m = ys.size();
        if (min(n, m) == 1) {
            cout << "1\n";
            continue;
        }

        vector<pair<int, int>> edges;
        vector<vector<int>> adj_list(n);
        for (auto [x, y] : points) {
            int u = lower_bound(xs.begin(), xs.end(), x) - xs.begin(), v = lower_bound(ys.begin(), ys.end(), y) - ys.begin();
            edges.emplace_back(u, v);
            adj_list[u].emplace_back(v);
        }

        auto [l, r, matches] = hopcroft_karp(n, m, edges);
        auto [cover_l, cover_r] = konig(n, m, adj_list, l, r);
        if (!cover_l.empty() && !cover_r.empty()) {
            cout << matches << "\n";
            continue;
        }

        if (cover_l.empty()) {
            vector<vector<int>> temp(m);
            for (auto [u, v] : edges) temp[v].emplace_back(u);
            swap(n, m);
            swap(l, r);
            adj_list = temp;
        }

        vector<vector<int>> adj_list_scc(n);
        vector<bool> valid(n, true);
        for (int v = 0; v < n; v++)
            for (int u : adj_list[v])
                if (!~r[u]) valid[v] = false;
                else if (r[u] != v) adj_list_scc[v].emplace_back(r[u]);

        auto [sccs, component, members] = tarjan(n, adj_list_scc);
        vector<bool> blocked(sccs + 1, false);
        for (int v = 0; v < n; v++) {
            blocked[component[v]] = blocked[component[v]] || !valid[v];
            for (int u : adj_list_scc[v])
                if (component[v] != component[u]) blocked[component[v]] = true;
        }

        bool extra = true;
        for (int i = 1; i <= sccs; i++)
            if (!blocked[i] && members[i].size() < n) extra = false;
        cout << matches + extra << "\n";
    }
}
