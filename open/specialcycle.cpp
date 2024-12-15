#include <bits/stdc++.h>
using namespace std;

struct DisjointSet {
    vector<int> sets;

    int find(int p) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p]));
    }

    bool unite(int p, int q) {
        int p_set = find(p), q_set = find(q);
        if (p_set != q_set) {
            sets[q_set] = p_set;
            return true;
        }
        return false;
    }

    DisjointSet(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

pair<int, vector<int>> micali_vazirani(int n, vector<pair<int, int>> edges) {
    vector<int> match(n, -1);
    vector<vector<int>> adj_list(n);
    for (auto [u, v] : edges) {
        adj_list[u].emplace_back(v);
        adj_list[v].emplace_back(u);
    }

    vector<int> visited(n, -1);
    int t = -1;
    bool change;
    do {
        change = false;
        DisjointSet dsu(n);
        vector<array<int, 2>> parent(n, {-1, -1});
        vector<array<int, 3>> fw(n, {-1, -1, -1}), bw(n, {-1, -1, -1});
        vector<int> order(n), anc(n), dir(n, -1), level(n, -1), depth(n, -1), lift(n, -1), state(n, -1);
        iota(order.begin(), order.end(), 0);
        iota(anc.begin(), anc.end(), 0);
        int count = 0;

        stack<int> s;
        for (int i = 0; i < n; i++)
            if (match[i] == -1) {
                state[i] = level[i] = 0;
                s.emplace(i);
            }

        while (!s.empty() && !change) {
            int v = s.top();
            s.pop();

            for (int u : adj_list[v]) {
                int p = dsu.find(v), q = dsu.find(u);
                if (p == q || state[q] == 1) continue;

                if (state[q] == -1) {
                    int m = match[q];
                    state[q] = 1;
                    state[m] = 0;
                    parent[q] = {v, u};
                    parent[m] = {u, m};
                    s.emplace(m);
                } else {
                    auto lca = [&]() {
                        t++;
                        int p_set = dsu.find(p), q_set = dsu.find(q);
                        while (p_set >= 0 || q_set >= 0) {
                            if (p_set != -1) {
                                if (visited[p_set] == t) return p_set;
                                visited[p_set] = t;
                                p_set = parent[p_set][0] >= 0 ? dsu.find(parent[p_set][0]) : parent[p_set][0];
                            }
                            swap(p_set, q_set);
                        }
                        return -1;
                    };

                    int a = lca();
                    if (a != -1) {
                        level[a] = ++count;
                        order.emplace_back(a);

                        auto dual_traversal = [&](bool rev) {
                            int x = !rev ? v : u, y = !rev ? u : v;

                            for (int x_set = dsu.find(x); x_set != a;) {
                                int z = parent[x_set][0], z_set = dsu.find(z);

                                if (state[x_set] == 1) s.emplace(x_set);

                                dir[x_set] = state[x_set] ^ rev;
                                anc[x_set] = a;
                                fw[x_set] = {z_set, z, parent[x_set][1]};
                                bw[x_set] = {dsu.find(y), y, x};

                                if (rev) swap(fw[x_set], bw[x_set]);

                                x = z;
                                y = parent[x_set][1];
                                x_set = z_set;
                            }
                        };
                        dual_traversal(false);
                        dual_traversal(true);

                        auto contract = [&](int r) {
                            for (int b = r; b != a; b = dsu.find(parent[b][0])) dsu.unite(a, b);
                        };
                        contract(p);
                        contract(q);
                    } else {
                        for (int i = order.size() - 1; ~i; i--) {
                            int r = order[i];
                            if (depth[r] == -1) {
                                a = anc[r];
                                depth[r] = depth[a] + 1;
                                lift[r] = a;
                                if (depth[r] > 1 && depth[a] - depth[lift[a]] == depth[lift[a]] - depth[lift[lift[a]]]) lift[r] = lift[lift[a]];
                            }
                        }

                        list<pair<int, int>> path;
                        for (int v_set = p; parent[v_set][0] != -1; v_set = dsu.find(parent[v_set][0])) path.emplace_front(parent[v_set][0], parent[v_set][1]);
                        path.emplace_front(-1, parent[p][0] == -1 ? p : dsu.find(path.front().first));
                        path.emplace_back(v, u);
                        for (int u_set = q; parent[u_set][0] != -1; u_set = dsu.find(parent[u_set][0])) path.emplace_back(parent[u_set][1], parent[u_set][0]);
                        path.emplace_back(parent[q][0] == -1 ? q : dsu.find(path.back().second), -1);

                        for (auto it = next(path.begin()); it != path.end();) {
                            auto [e, f] = *prev(it);
                            auto [g, h] = *it;

                            if (f == g) {
                                it++;
                                continue;
                            }

                            bool rev = h == -1 || match[g] == h;
                            if (rev) {
                                swap(e, h);
                                swap(f, g);
                            }
                            while (level[anc[g]] < level[f]) g = level[lift[g]] < level[f] ? lift[g] : anc[g];

                            list<pair<int, int>> temp;
                            auto add = [&](auto &w) {
                                for (int i = g; i != f; i = w[i][0]) temp.emplace_front(w[i][1], w[i][2]);
                            };
                            add(!dir[g] ? fw : bw);

                            if (!rev) it = path.insert(it, temp.begin(), temp.end());
                            else {
                                for (auto &[x, y] : temp) swap(x, y);
                                it = path.insert(it, temp.rbegin(), temp.rend());
                            }
                        }
                        path.pop_front();
                        path.pop_back();

                        for (auto it = path.begin();; it = next(it, 2)) {
                            match[it->first] = it->second;
                            match[it->second] = it->first;
                            if (next(it) == path.end()) break;
                        }
                        change = true;
                    }
                }
                if (change) break;
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

    for (auto [V, U] : edges) {
        vector<pair<int, int>> e;
        for (auto [v, u] : edges)
            if (v != V && u != U) e.emplace_back(v, u);

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

            for (auto [v, u] : edges) {
                link[v] = u;
                link[u] = v;
            }

            vector<int> full;
            int curr = V;
            do {
                full.emplace_back(next[curr]);
                curr = match[curr];
                full.emplace_back(next[curr]);
                curr = link[curr];
            } while (curr != V);

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
