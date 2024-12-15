#include <bits/stdc++.h>
using namespace std;

struct DisjointSet {
    vector<int> sets, parent;

    int find(int p) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p]));
    }

    bool unite(int p, int q) {
        int p_set = find(p), q_set = find(q);
        if (p_set != q_set) {
            sets[q_set] = p_set;
            parent[q] = p;
            return true;
        }
        return false;
    }

    DisjointSet(int n) : sets(n), parent(n, -1) {
        iota(sets.begin(), sets.end(), 0);
    }
};

pair<int, vector<int>> micali_vazirani(int n, vector<pair<int, int>> edges) {
    vector<int> match(n, -1);

    vector<vector<array<int, 3>>> adj_list(n);
    for (auto [u, v] : edges) {
        adj_list[u].push_back({v, (int) adj_list[v].size(), 0});
        adj_list[v].push_back({u, (int) adj_list[u].size() - 1, 0});
    }

    bool change;
    do {
        change = false;
        for (auto &al : adj_list)
            for (auto &[u, index, label] : al) label = 0;

        vector<int> even(n, 1e9), odd(n, 1e9);
        vector<vector<int>> vertices(n);
        auto set_level = [&](int v, int level) {
            if (level & 1) odd[v] = level;
            else even[v] = level;

            vertices[level].emplace_back(v);
        };

        for (int u = 0; u < n; u++)
            if (match[u] == -1) set_level(u, 0);

        auto min_level = [&](int v) {
            return min(even[v], odd[v]);
        };

        auto tenacity = [&](int u, int v) {
            if (match[u] == v) return odd[u] + odd[v] + 1;
            return even[u] + even[v] + 1;
        };

        DisjointSet dsu(n);
        int mark_count = 1;
        vector<int> count(n, 0), mark(n, 0), indices(n, 0);
        vector<bool> visited(n, false);
        vector<vector<int>> predecessors(n);
        vector<vector<pair<int, int>>> children(n), bridges(2 * (n + 1));
        vector<pair<pair<int, int>, pair<int, int>>> support_bridge(n);
        for (int i = 0; i < n && !change; i++) {
            for (int v : vertices[i])
                for (auto &[u, index, label] : adj_list[v])
                    if (!label && !((odd[v] == i) ^ (match[v] == u))) {
                        if (min_level(u) >= i + 1) {
                            label = 1;
                            adj_list[u][index][2] = 1;

                            if (min_level(u) > i + 1) set_level(u, i + 1);
                            predecessors[u].emplace_back(v);
                        } else {
                            label = 2;
                            adj_list[u][index][2] = 2;

                            int j = tenacity(u, v);
                            if (j < 2 * (n + 1)) bridges[j].emplace_back(v, u);
                        }
                    }

            for (auto [s, t] : bridges[2 * i + 1]) {
                int vl = dsu.find(s), vr = dsu.find(t);
                if (visited[vl] || visited[vr] || vl == vr) continue;

                vector<int> support;
                auto double_dfs = [&]() -> pair<int, int> {
                    vector<int> tl{vl}, tr{vr};
                    support.emplace_back(vl);
                    support.emplace_back(vr);

                    mark[vl] = ++mark_count;
                    mark[vr] = ++mark_count;
                    for (;;) {
                        if (!min_level(tl.back()) && !min_level(tr.back())) return {tl.back(), tr.back()};

                        auto dfs = [&](bool left) {
                            auto &t1 = left ? tl : tr, &t2 = left ? tr : tl;
                            int m = left ? mark[vl] : mark[vr], v = t1.back();
                            for (int &k = indices[v]; k < predecessors[v].size(); k++) {
                                int u = predecessors[v][k];
                                if (visited[u]) continue;

                                int u_set = dsu.find(u);
                                if (!mark[u_set]) {
                                    mark[u_set] = m;
                                    t1.emplace_back(u_set);
                                    support.emplace_back(u_set);
                                    children[v].emplace_back(u, u_set);
                                    return -1;
                                } else if (u_set == t2.back()) children[v].emplace_back(u, u_set);
                            }
                            t1.pop_back();

                            if (t1.empty()) {
                                if (t2.size() == 1) {
                                    mark[t2[0]] = 0;
                                    return t2[0];
                                }

                                mark[t2.back()] = m;
                                t1.emplace_back(t2.back());
                                t2.pop_back();
                            }
                            return -1;
                        };

                        int bottleneck = min_level(tl.back()) >= min_level(tr.back()) ? dfs(true) : dfs(false);
                        if (bottleneck != -1) return {bottleneck, bottleneck};
                    }
                };
                auto [x, y] = double_dfs();

                for (int v : support) {
                    if (v == y) continue;
                    support_bridge[v] = {{s, t}, {vl, vr}};
                    dsu.unite(y, v);

                    set_level(v, 2 * i + 1 - min_level(v));
                    for (auto [u, index, label] : adj_list[v])
                        if (even[v] > odd[v] && label == 2 && match[v] != u) {
                            int j = tenacity(u, v);
                            if (j < 2 * (n + 1)) bridges[j].emplace_back(v, u);
                        }
                }
                if (x == y) continue;

                queue<int> q;
                auto augment = [&](int u, int v) {
                    if (visited[u] || visited[v] || match[u] == v || match[v] == u) return;
                    visited[u] = visited[v] = true;
                    q.emplace(u);
                    q.emplace(v);
                    match[u] = v;
                    match[v] = u;
                };

                bool first = true;
                auto find_path = [&](auto &&self1, int x, int y) {
                    if (x == y) return;

                    if (!first && odd[x] > even[x]) {
                        int p = predecessors[x][0], k = 0;
                        while (dsu.find(predecessors[p][k]) != dsu.find(p)) k++;
                        x = predecessors[p][k];
                        augment(p, x);
                        self1(self1, x, y);
                    } else {
                        first = false;

                        auto [a, b] = support_bridge[x].first;
                        auto [a_set, b_set] = support_bridge[x].second;
                        if (mark[x] == (mark[a_set] ^ 1) || mark[x] == mark[b_set]) {
                            swap(a, b);
                            swap(a_set, b_set);
                        }
                        augment(a, b);

                        auto dfs = [&](auto &&self2, int v, int v_set) {
                            if (v_set == x) {
                                self1(self1, v, v_set);
                                return true;
                            }

                            for (auto [u, u_set] : children[v_set])
                                if ((u_set == x || mark[u_set] == mark[v_set]) && self2(self2, u, u_set)) {
                                    self1(self1, v, v_set);
                                    augment(v_set, u);
                                    return true;
                                }
                            return false;
                        };
                        dfs(dfs, a, a_set);
                        x = dsu.parent[x];
                        dfs(dfs, b, b_set);
                        self1(self1, x, y);
                    }
                };
                find_path(find_path, x, y);
                change = true;
                while (!q.empty()) {
                    int v = q.front();
                    q.pop();

                    for (auto [u, index, label] : adj_list[v])
                        if (label == 1 && min_level(u) > min_level(v) && !visited[u] && ++count[u] == predecessors[u].size()) {
                            visited[u] = true;
                            q.emplace(u);
                        }
                }
            }
        }
    } while (change);

    int matches = (n - count(match.begin(), match.end(), -1)) / 2;
    return {matches, match};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;

    vector<int> a(m), b(m);
    for (int i = 0; i < m; i++) {
        cin >> a[i] >> b[i];
        a[i]--;
        b[i]--;
    }

    vector<vector<int>> adj_list(n);
    vector<pair<int, int>> edges(k);
    for (int i = 0; i < k; i++) {
        adj_list[a[i]].emplace_back(2 * i);
        adj_list[b[i]].emplace_back(2 * i + 1);
        edges[i] = {2 * i, 2 * i + 1};
    }

    vector<int> degree(n, 0);
    int id = 2 * k;
    for (int i = 0; i < n; i++)
        if (adj_list[i].empty()) {
            adj_list[i].emplace_back(id++);
            adj_list[i].emplace_back(id++);
        } else degree[i] = adj_list[i].size();

    vector<int> next(id, -1);
    for (int v = 0; v < n; v++)
        for (int u : adj_list[v]) next[u] = v;

    for (auto [U, V] : edges) {
        vector<pair<int, int>> e;
        for (auto [u, v] : edges)
            if (u != U && v != V) e.emplace_back(u, v);

        for (int i = k; i < m; i++)
            if (degree[a[i]] < 2 && degree[b[i]] < 2)
                for (int v : adj_list[a[i]])
                    for (int u : adj_list[b[i]]) e.emplace_back(v, u);

        for (int i = 0; i < n; i++)
            if (adj_list[i].size() == 2) e.emplace_back(adj_list[i][0], adj_list[i][1]);

        auto [matches, match] = micali_vazirani(id, e);
        if (2 * matches == id) {
            vector<int> link(id, -1);
            for (int i = 0; i < n; i++)
                if (adj_list[i].size() == 2) {
                    link[adj_list[i][0]] = adj_list[i][1];
                    link[adj_list[i][1]] = adj_list[i][0];
                }

            for (auto [u, v] : edges) {
                link[u] = v;
                link[v] = u;
            }

            vector<int> full;
            int curr = U;
            do {
                full.emplace_back(next[curr]);
                curr = match[curr];
                full.emplace_back(next[curr]);
                curr = link[curr];
            } while (curr != U);

            vector<int> special;
            for (int v : full) {
                for (int u : special)
                    if (v == u) {
                        while (special.back() != v) special.pop_back();
                        goto next;
                    }
                special.emplace_back(v);
                next:;
            }

            cout << special.size() << "\n";
            for (int v : special) cout << v + 1 << "\n";
            exit(0);
        }
    }
    cout << -1;
}
