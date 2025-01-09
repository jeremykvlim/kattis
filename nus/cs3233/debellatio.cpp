#include <bits/stdc++.h>
using namespace std;

struct PersistentDisjointSets {
    vector<int> sets;
    deque<pair<int, int>> history;

    int find(int p) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p]));
    }

    bool unite(int p, int q) {
        int p_set = find(p), q_set = find(q);
        if (p_set != q_set) {
            sets[q_set] = p_set;
            history.emplace_back(q_set, q_set);
            return true;
        }
        return false;
    }

    int record() {
        return history.size();
    }

    void restore(int version = 0) {
        while (record() > version) {
            sets[history.back().first] = history.back().second;
            history.pop_back();
        }
    }

    void delete_history(int version = 0) {
        history.resize(version);
    }

    void reset() {
        iota(sets.begin(), sets.end(), 0);
    }

    PersistentDisjointSets(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

struct ForwardStar {
    vector<int> head, next;

    ForwardStar() {}
    ForwardStar(int n, int m) : head(n, -1), next(m) {}
    void add_edge(int u, int edge_id) {
        next[edge_id] = head[u];
        head[u] = edge_id;
    }
};

pair<int, vector<int>> gabow(int n, vector<pair<int, int>> edges) {
    vector<pair<int, int>> adj_list(2 * edges.size());
    vector<int> degree(n + 2, 0);
    for (auto [u, v] : edges) {
        degree[u + 1]++;
        degree[v + 1]++;
    }
    for (int i = 1; i < n + 2; i++) degree[i] += degree[i - 1];
    for (auto [u, v] : edges) {
        adj_list[degree[u]++] = {u, v};
        adj_list[degree[v]++] = {v, u};
    }
    for (int i = n + 1; i; i--) degree[i] = degree[i - 1];
    degree[0] = 0;

    int h = n / 2;
    vector<int> match(n + 1, 0), potential(n + 1, 1), label(n + 1, 0);
    vector<pair<int, int>> link(n + 1, {0, 0});
    ForwardStar list(h + 1, adj_list.size()), blossom(n + 1, n + 1);
    PersistentDisjointSets pdsu(n + 1);

    int matches = 0;
    while (2 * matches < n - 1) {
        int outer = 1, p_augment = INT_MAX, p_curr = 0, count = -2, version = 0;
        queue<int> q;
        for (int u = 1; u <= n; u++) {
            if (!match[u]) {
                q.emplace(u);
                label[u] = u;
            } else label[u] = 0;
        }

        auto make_outer = [&](int x, int y, int z, int p) {
            link[z] = {x, y};
            label[z] = label[x];
            potential[z] = p_curr + 1 + p;
            q.emplace(z);
        };

        auto grow = [&](int x, int y, int z) {
            label[y] = -1;
            potential[y] = p_curr;
            make_outer(x, y, z, 0);
        };

        auto contract = [&](int x, int y) {
            int base_x = pdsu.find(x), base_y = pdsu.find(y), l = count--;
            label[match[base_x]] = label[match[base_y]] = l;

            int lca;
            for (;;) {
                if (match[base_y]) swap(base_x, base_y);
                base_x = lca = pdsu.find(link[base_x].first);
                if (label[match[base_x]] == l) break;
                label[match[base_x]] = l;
            }

            base_x = pdsu.find(x), base_y = pdsu.find(y);
            for (int base : {base_x, base_y})
                for (; base != lca; base = pdsu.sets[link[base].first]) {
                    int z = match[base];
                    make_outer(x, y, z, p_curr - potential[z]);

                    pdsu.sets[base] = pdsu.sets[z] = lca;
                    pdsu.history.emplace_back(base, lca);
                    pdsu.history.emplace_back(z, lca);
                }
        };

        for (;;) {
            while (!q.empty()) {
                int x = q.front(), l_x = label[x], p_x = potential[x], base_x = pdsu.find(x);
                q.pop();

                for (int i = degree[x]; i < degree[x + 1]; i++) {
                    int y = adj_list[i].second, l_y = label[y], p_y = potential[y], base_y = pdsu.find(y);

                    if (l_y > 0) {
                        int p_mid = (p_x + p_y) / 2;
                        if (l_x != l_y) {
                            if (p_mid == p_curr) goto done;
                            p_augment = min(p_augment, p_mid);
                        } else {
                            if (base_x == base_y) continue;
                            if (p_mid == p_curr) {
                                contract(x, y);
                                base_x = pdsu.find(x);
                            } else if (p_mid <= h) list.add_edge(p_mid, i);
                        }
                    } else if (!l_y) {
                        int p_next = p_x + 1;
                        if (p_next == p_curr) grow(x, y, match[y]);
                        else if (p_next <= h) list.add_edge(p_next, i);
                    }
                }
            }

            int limit = min(h + 1, p_augment);
            for (p_curr++; p_curr <= limit; p_curr++) {
                version = pdsu.record();
                if (p_curr == limit) break;

                bool updated = false;
                for (int i = list.head[p_curr]; ~i; i = list.next[i]) {
                    auto [x, y] = adj_list[i];
                    int l_x = label[x], p_x = potential[x], base_x = pdsu.find(x),
                        l_y = label[y], p_y = potential[y], base_y = pdsu.find(y);

                    if (l_y > 0) {
                        if (p_curr != (p_x + p_y) / 2 || base_x == base_y) continue;
                        if (l_x != l_y) {
                            p_augment = p_curr;
                            goto next;
                        } else {
                            contract(x, y);
                            updated = true;
                        }
                    } else if (!l_y) {
                        int z = match[y];
                        grow(x, y, z);
                        updated = true;
                    }
                }
                list.head[p_curr] = -1;
                if (updated) goto next;
            }
            if (p_curr > h) return {matches, match};
            next:;
            if (p_curr == p_augment) break;
        }
        done:;

        for (int u = 1; u <= n; u++) {
            if (label[u] > 0) potential[u] -= p_curr;
            else if (label[u] < 0) potential[u] = p_curr + 1 - potential[u];
        }
        pdsu.reset();
        pdsu.delete_history(version);
        pdsu.restore();

        for (int u = 1; u <= n; u++) {
            label[u] = 0;
            blossom.add_edge(pdsu.find(u), u);
        }

        stack<int> s;
        auto dfs = [&](auto &&self1, int x, int base_x) -> bool {
            int l_base_x = label[base_x], p_x = potential[x];
            for (int i = degree[x]; i < degree[x + 1]; ++i) {
                int y = adj_list[i].second, p_y = potential[y];
                if (p_x != -p_y) continue;

                int base_y = pdsu.find(y), l_base_y = label[base_y];
                if (l_base_y > 0) {
                    if (l_base_x >= l_base_y) continue;
                    int size = s.size();
                    for (int base = base_y; base != base_x; base = pdsu.find(link[base].first)) {
                        int z = match[base], base_z = pdsu.find(z);
                        link[base_z] = {x, y};
                        pdsu.sets[base] = pdsu.sets[base_z] = base_x;
                        s.emplace(base_z);
                    }

                    while (s.size() > size) {
                        int base_v = s.top();
                        s.pop();

                        for (int v = blossom.head[base_v]; ~v; v = blossom.next[v]) {
                            if (!self1(self1, v, base_x)) continue;
                            while (s.size() > size) s.pop();
                            return true;
                        }
                    }
                } else if (!l_base_y) {
                    label[base_y] = -1;
                    int z = match[base_y];

                    auto rematch = [&](auto &&self2, int u, int v) {
                        int t = match[u];
                        match[u] = v;
                        if (match[t] != u) return;

                        auto [a, b] = link[u];
                        if (pdsu.find(b) == b) {
                            match[t] = a;
                            self2(self2, match[t], t);
                        } else {
                            self2(self2, a, b);
                            self2(self2, b, a);
                        }
                    };

                    if (!z) {
                        rematch(rematch, x, y);
                        rematch(rematch, y, x);
                        return true;
                    }

                    int base_z = pdsu.find(z);
                    link[base_z] = {x, y};
                    label[base_z] = outer++;
                    for (int v = blossom.head[base_z]; ~v; v = blossom.next[v])
                        if (self1(self1, v, base_z)) return true;
                }
            }
            return false;
        };

        for (int u = 1; u <= n; u++)
            if (!match[u]) {
                int base_u = pdsu.find(u);
                if (label[base_u]) continue;

                label[base_u] = outer++;
                for (int v = blossom.head[base_u]; ~v; v = blossom.next[v]) {
                    if (!dfs(dfs, v, base_u)) continue;
                    matches++;
                    break;
                }
            }

        pdsu.reset();
        pdsu.delete_history();
        fill(potential.begin(), potential.end(), 1);
        fill(blossom.head.begin(), blossom.head.end(), -1);
        fill(list.head.begin(), list.head.end(), -1);
    }
    return {matches, match};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<pair<int, int>> edges(m);
    for (auto &[a, b] : edges) cin >> a >> b;

    auto [matches, match] = gabow(n, edges);
    if (matches * 2 == n) cout << "2\n" << flush;
    else {
        cout << "1\n" << flush;
        for (int a = 1; a <= n; a++)
            if (!match[a]) {
                cout << a << "\n" << flush;
                break;
            }
    }

    int a;
    while (cin >> a && a) cout << match[a] << "\n" << flush;
}
