#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets, parent;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            parent[v_set] = sets[v_set] = u_set;
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n), parent(n, -1) {
        iota(sets.begin(), sets.end(), 0);
    }
};

template<typename T>
pair<T, vector<int>> edmonds_dense(int n, vector<vector<pair<int, T>>> adj_list_transpose, int root = 0) {
    for (auto &neighbors : adj_list_transpose) sort(neighbors.begin(), neighbors.end(), [&](const auto &p1, const auto &p2) { return p1.second > p2.second; });

    DisjointSets dsu(n);
    vector<T> cost(n, 0), loss(n, 0);
    vector<pair<int, int>> chosen_edge(n, {-1, -1});
    vector<int> visited(n, 0);
    vector<bool> cycle(n, false);
    stack<pair<vector<int>, vector<pair<int, int>>>> history;
    for (T len = 0;;) {
        int root_set = dsu.find(root);
        for (int v = 0; v < n; v++)
            if (v == dsu.find(v)) {
                cost[v] = v != root_set ? numeric_limits<T>::max() : 0;
                chosen_edge[v] = {-1, -1};
            }

        for (int v = 0; v < n; v++) {
            while (!adj_list_transpose[v].empty() && dsu.find(v) == dsu.find(adj_list_transpose[v].back().first)) adj_list_transpose[v].pop_back();
            if (!adj_list_transpose[v].empty()) {
                auto [u, w] = adj_list_transpose[v].back();
                T c = w - loss[v];
                int v_set = dsu.find(v);
                if (cost[v_set] > c) {
                    cost[v_set] = c;
                    chosen_edge[v_set] = {u, v};
                }
            }
        }

        for (int v = 0; v < n; v++)
            if (v == dsu.find(v) && cost[v] == numeric_limits<T>::max()) return {-1, {}};

        int count = 1;
        vector<vector<int>> cycles;
        fill(cycle.begin(), cycle.end(), false);
        fill(visited.begin(), visited.end(), 0);
        visited[root_set] = count++;
        for (int v = 0; v < n; v++)
            if (v == dsu.find(v) && !visited[v]) {
                int u = v;
                stack<int> s;
                while (!visited[u]) {
                    visited[u] = count;
                    s.emplace(u);
                    u = dsu.find(chosen_edge[u].first);
                }

                if (visited[u] == count) {
                    cycle[u] = true;
                    cycles.emplace_back(vector<int>{u});
                    while (s.top() != u) {
                        cycle[s.top()] = true;
                        cycles.back().emplace_back(s.top());
                        s.pop();
                    }
                }
                count++;
            }

        if (!cycles.empty()) {
            for (int v = 0; v < n; v++) {
                int v_set = dsu.find(v);
                if (cycle[v_set]) {
                    if (v == v_set) len += cost[v_set];
                    loss[v] += cost[v_set];
                }
            }

            for (auto &c : cycles) {
                vector<pair<int, int>> edges;
                for (int ci : c) {
                    if (ci != c[0]) dsu.unite(c[0], ci);
                    edges.emplace_back(chosen_edge[ci]);
                }
                history.emplace(c, edges);
            }
            continue;
        }

        for (int v = 0; v < n; v++)
            if (v == dsu.find(v)) len += cost[v];

        vector<int> mst(n, -1);
        for (int v = 0; v < n; v++)
            if (v == dsu.find(v)) {
                if (v == dsu.find(root)) chosen_edge[v] = {-1, root};
                else mst[chosen_edge[v].second] = chosen_edge[v].first;
            }

        while (!history.empty()) {
            auto [c, edges] = history.top();
            history.pop();

            auto [u, v] = chosen_edge[c[0]];
            for (int ci : c) visited[ci] = count;
            int t = v;
            while (t != -1 && visited[t] != count) t = dsu.parent[t];
            count++;

            for (int i = 0; i < c.size(); i++) 
                if (c[i] == t) {
                    mst[v] = u;
                    chosen_edge[c[i]] = {u, v};
                } else {
                    auto [x, y] = edges[i];
                    mst[y] = x;
                    chosen_edge[c[i]] = {x, y};
                }
        }
        mst[root] = root;
        return {len, mst};
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<pair<int, long long>>> adj_list_transpose(n + 1);
    for (int i = 1; i <= n; i++) {
        int x;
        long long s;
        cin >> x >> s;

        for (int j = 0; j <= n; j++) {
            long long a;
            cin >> a;

            if (i != j) adj_list_transpose[i].emplace_back(j, j == x ? min(a, s) : a);
        }
    }

    auto [len, mst] = edmonds_dense(n + 1, adj_list_transpose);
    cout << len;
}
