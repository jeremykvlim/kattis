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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, q;
    cin >> n >> m >> q;

    vector<int> k(m + 1);
    vector<array<int, 10>> p(m + 1);
    vector<array<int, 3>> queries(q);
    vector<vector<int>> weeks(n + 1);
    for (auto &[b, t, x] : queries) {
        cin >> b >> t >> x;

        if (!b) {
            k[x] = t;
            for (int i = 0; i < t; i++) {
                cin >> p[x][i];

                weeks[p[x][i]].emplace_back(x);
            }
        }
    }

    int nodes = n;
    vector<vector<int>> id(n + 1);
    vector<array<pair<int, int>, 10>> links(m + 1);
    for (int pi = 1; pi <= n; pi++)
        if (!weeks[pi].empty()) {
            sort(weeks[pi].begin(), weeks[pi].end());
            int w = weeks[pi].size();
            id[pi].resize(w);
            for (int j = 0; j < w; j++) id[pi][j] = ++nodes;

            for (int j = 0; j < w;) {
                int x = weeks[pi][j], u = !j ? pi : id[pi][j - 1];
                for (; j < w && x == weeks[pi][j]; j++);
                int v = id[pi][j - 1];
                for (int i = 0; i < k[x]; i++)
                    if (p[x][i] == pi) links[x][i] = {u, v};
            }
        }

    vector<long long> a(nodes + 1, 0);
    iota(a.begin(), a.begin() + n + 1, 0);
    OfflineDynamicGraph<long long> odg(a);
    vector<int> edge_id(nodes + 1);
    for (int pi = 1; pi <= n; pi++)
        for (int j = 0; j < id[pi].size(); j++) {
            int v = id[pi][j];
            edge_id[v] = odg.add_edge(v, !j ? pi : id[pi][j - 1]);
        }

    for (auto [b, t, x] : queries)
        if (!b) {
            for (int i = 0; i < k[x]; i++) {
                int u = links[x][(i + 1) % k[x]].first, v = links[x][i].second;
                odg.delete_edge(edge_id[v]);
                if (u) edge_id[v] = odg.add_edge(u, v);
            }
        } else {
            int j = upper_bound(weeks[t].begin(), weeks[t].end(), x) - weeks[t].begin(), v = !j ? t : id[t][j - 1];
            odg.query([v](auto &amt) {
                cout << amt.component_sum(v) << "\n";
            });
        }

    odg.process();
}