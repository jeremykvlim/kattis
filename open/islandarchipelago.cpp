#include <bits/stdc++.h>
using namespace std;

template <typename T, typename U>
struct AntiMonopolyTree {
    vector<int> parent, size;
    vector<T> weight;
    vector<U> sum;

    AntiMonopolyTree(const vector<U> &a) : parent(a.size(), -1), size(a.size(), 1), weight(a.size(), numeric_limits<T>::max()), sum(a) {}

    pair<T, int> path_max(int u, int v) {
        upward_maintain(u);
        upward_maintain(v);

        T max_w = numeric_limits<T>::min();
        int t = -1;
        while (u != v) {
            if (size[u] > size[v]) swap(u, v);
            if (weight[u] == numeric_limits<T>::max()) return {numeric_limits<T>::max(), -1};
            if (max_w < weight[u]) {
                max_w = weight[u];
                t = u;
            }
            u = parent[u];
        }
        return {max_w, t};
    }

    bool connected(int u, int v) {
        return u == v || path_max(u, v).second != -1;
    }

    void upward_maintain(int v) {
        while (~parent[v]) {
            int p = parent[v];
            if (3 * size[v] <= 2 * size[p]) {
                v = p;
                continue;
            }

            sum[p] -= sum[v];
            size[p] -= size[v];
            parent[v] = parent[p];
            if (weight[v] < weight[p]) {
                sum[v] += sum[p];
                size[v] += size[p];
                swap(weight[v], weight[p]);
                parent[p] = v;
            }
        }
    }

    int root(int v) {
        while (~parent[v]) v = parent[v];
        return v;
    }

    void cut(int v) {
        for (int p = parent[v]; ~p; p = parent[p]) {
            size[p] -= size[v];
            sum[p] -= sum[v];
        }
        parent[v] = -1;
        weight[v] = numeric_limits<T>::max();
    }

    bool add(int u, int v, T w) {
        if (u == v) return false;

        auto [max_w, t] = path_max(u, v);
        bool merged = max_w == numeric_limits<T>::max();
        if (!merged) {
            if (w >= max_w) return false;
            cut(t);
        }

        int du = 0, dv = 0;
        U su = 0, sv = 0;
        while (~u && ~v) {
            if (w >= weight[u]) {
                int p = parent[u];
                if (~p) {
                    size[p] += du;
                    sum[p] += su;
                }
                u = p;
            } else if (w >= weight[v]) {
                int p = parent[v];
                if (~p) {
                    size[p] += dv;
                    sum[p] += sv;
                }
                v = p;
            } else {
                if (size[u] > size[v]) {
                    swap(u, v);
                    swap(du, dv);
                    swap(su, sv);
                }

                du -= size[u];
                su -= sum[u];
                dv += size[u];
                sv += sum[u];
                size[v] += size[u];
                sum[v] += sum[u];
                w = exchange(weight[u], w);
                u = exchange(parent[u], v);
                if (~u) {
                    size[u] += du;
                    sum[u] += su;
                }
            }
        }

        if (~v)
            for (v = parent[v]; ~v; v = parent[v]) {
                size[v] += dv;
                sum[v] += sv;
            }
        return merged;
    }

    bool remove(int u, int v, T w) {
        auto [max_w, t] = path_max(u, v);
        if (max_w != w) return false;

        cut(t);
        return true;
    }

    U component_sum(int v) {
        upward_maintain(v);
        return sum[root(v)];
    }
};

template <typename V>
struct OfflineDynamicGraph {
    AntiMonopolyTree<int, V> amt;
    vector<array<int, 3>> edges;
    vector<pair<int, function<void(AntiMonopolyTree<int, V> &)>>> queries;

    OfflineDynamicGraph(const vector<V> &a) : amt(a) {}

    int add_edge(int u, int v) {
        edges.push_back({u, v, 0});
        return edges.size() - 1;
    }

    void delete_edge(int e) {
        auto [u, v, w] = edges[e];
        edges[e][2] = -edges.size();
        edges.push_back({u, v, 1});
    }

    template <typename F>
    void query(F &&f) {
        queries.emplace_back(edges.size(), f);
    }

    void process() {
        int q = 0;
        for (int i = 0; i < edges.size(); i++) {
            for (; q < queries.size() && queries[q].first == i; q++) queries[q].second(amt);

            auto [u, v, w] = edges[i];
            if (w == 1) amt.remove(u, v, -i);
            else amt.add(u, v, w ? w : -edges.size() - 1);
        }
        for (; q < queries.size(); q++) queries[q].second(amt);
    }
};

template <typename T>
struct AffineFunction {
    T m, c;

    AffineFunction(T m = 0, T c = 0) : m(m), c(c) {}

    T operator()(T x) {
        return m * x + c;
    }

    AffineFunction & operator+=(const AffineFunction &f) {
        m += f.m;
        c += f.c;
        return *this;
    }

    AffineFunction & operator-=(const AffineFunction &f) {
        m -= f.m;
        c -= f.c;
        return *this;
    }

    friend AffineFunction operator+(AffineFunction f1, const AffineFunction &f2) {
        return f1 += f2;
    }

    friend AffineFunction operator-(AffineFunction f1, const AffineFunction &f2) {
        return f1 -= f2;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> order(m, -1);
    vector<vector<int>> indices(n, vector<int>(n, -1));
    vector<pair<int, int>> changes;
    for (int i = 0; i < m; i++) {
        char type;
        cin >> type;

        if (type == '!') {
            int r, c;
            cin >> r >> c;
            r--;
            c--;

            if (!~indices[r][c]) {
                indices[r][c] = changes.size();
                changes.emplace_back(r, c);
            }
            order[i] = indices[r][c];
        }
    }

    int k = changes.size();
    vector<array<int, 3>> links;
    vector<vector<int>> link_indices(k);
    for (int a = 0; a < k; a++) {
        auto [r, c] = changes[a];

        if (c + 1 < n) {
            int b = indices[r][c + 1];
            if (~b) {
                links.push_back({a, b, -1});
                link_indices[a].emplace_back(links.size() - 1);
                link_indices[b].emplace_back(links.size() - 1);
            }
        }

        if (r + 1 < n) {
            int b = indices[r + 1][c];
            if (~b) {
                links.push_back({a, b, -1});
                link_indices[a].emplace_back(links.size() - 1);
                link_indices[b].emplace_back(links.size() - 1);
            }
        }
    }

    vector<array<int, 5>> blocks;
    vector<vector<int>> block_indices(k);
    for (int a = 0; a < k; a++) {
        auto [row, col] = changes[a];
        if (row + 1 < n && col + 1 < n) {
            int b = indices[row][col + 1], c = indices[row + 1][col], d = indices[row + 1][col + 1];
            if (!~b || !~c || !~d) continue;
            blocks.push_back({a, b, c, d, -1});
            block_indices[a].emplace_back(blocks.size() - 1);
            block_indices[b].emplace_back(blocks.size() - 1);
            block_indices[c].emplace_back(blocks.size() - 1);
            block_indices[d].emplace_back(blocks.size() - 1);
        }
    }

    vector<AffineFunction<int>> value(k + 1 + links.size() + blocks.size());
    for (int i = 0; i < k; i++) value[i] = {1, 1};
    value[k] = {0, -k - 1};
    for (int i = 0; i < links.size(); i++) value[i + k + 1] = {-1, 0};
    for (int i = 0; i < blocks.size(); i++) value[i + k + 1 + links.size()] = {1, 0};

    OfflineDynamicGraph<AffineFunction<int>> odg(value);
    int x = k, y = k;
    auto update = [&](const AffineFunction<int> &f, int d) {
        if (f.c <= 0) return;
        x += d;
        if (f.m == 1) y += d;
    };

    auto add = [&](int u, int v) {
        odg.query([&, u, v](auto &amt) {
            if (amt.connected(u, v)) return;
            auto s1 = amt.component_sum(u), s2 = amt.component_sum(v);
            update(s1, -1);
            update(s2, -1);
            update(s1 + s2, 1);
        });
        return odg.add_edge(u, v);
    };

    auto remove = [&](int e) {
        auto [u, v, w] = odg.edges[e];
        odg.delete_edge(e);
        odg.query([&, u, v](auto &amt) {
            if (amt.connected(u, v)) return;
            auto s1 = amt.component_sum(u), s2 = amt.component_sum(v);
            update(s1 + s2, -1);
            update(s1, 1);
            update(s2, 1);
        });
    };

    vector<int> edge_id(k);
    vector<bool> land(k, false);
    for (int i = 0; i < k; i++) edge_id[i] = add(i, k);
    for (int i : order) {
        if (~i) {
            if (!land[i]) {
                remove(edge_id[i]);
                land[i] = true;

                for (int j : link_indices[i]) {
                    auto &[a, b, e] = links[j];
                    if (land[a] && land[b]) {
                        e = add(a, j + k + 1);
                        add(j + k + 1, b);
                    }
                }

                for (int j : block_indices[i]) {
                    auto &[a, b, c, d, e] = blocks[j];
                    if (land[a] && land[b] && land[c] && land[d]) e = add(a, j + k + 1 + links.size());
                }
            } else {
                for (int j : link_indices[i]) {
                    auto &[a, b, e] = links[j];
                    if (~e) {
                        remove(e);
                        remove(e + 1);
                        e = -1;
                    }
                }

                for (int j : block_indices[i]) {
                    auto &[a, b, c, d, e] = blocks[j];
                    if (~e) {
                        remove(e);
                        e = -1;
                    }
                }

                land[i] = false;
                edge_id[i] = add(i, k);
            }
        } else odg.query([&](auto &) { cout << x << " " << y << "\n"; });
    }
    odg.process();
}
