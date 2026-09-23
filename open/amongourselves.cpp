#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct AntiMonopolyTree {
    vector<int> parent, size;
    vector<T> weight;

    AntiMonopolyTree(int n) : parent(n, -1), size(n, 1), weight(n, numeric_limits<T>::max()) {}

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

            size[p] -= size[v];
            parent[v] = parent[p];
            if (weight[v] < weight[p]) {
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
        for (int p = parent[v]; ~p; p = parent[p]) size[p] -= size[v];
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
        while (~u && ~v) {
            if (w >= weight[u]) {
                int p = parent[u];
                if (~p) size[p] += du;
                u = p;
            } else if (w >= weight[v]) {
                int p = parent[v];
                if (~p) size[p] += dv;
                v = p;
            } else {
                if (size[u] > size[v]) {
                    swap(u, v);
                    swap(du, dv);
                }

                du -= size[u];
                dv += size[u];
                size[v] += size[u];
                w = exchange(weight[u], w);
                u = exchange(parent[u], v);
                if (~u) size[u] += du;
            }
        }

        if (~v)
            for (v = parent[v]; ~v; v = parent[v]) size[v] += dv;
        return merged;
    }

    bool remove(int u, int v, T w) {
        auto [max_w, t] = path_max(u, v);
        if (max_w != w) return false;

        cut(t);
        return true;
    }
};

struct OfflineDynamicGraph {
    AntiMonopolyTree<int> amt;
    vector<array<int, 3>> edges;
    vector<pair<int, function<void(AntiMonopolyTree<int> &)>>> queries;

    OfflineDynamicGraph(int n) : amt(n) {}

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

    int n, m, k, t;
    cin >> n >> m >> k >> t;

    vector<array<int, 3>> edges(m);
    for (auto &[a, b, s] : edges) {
        string S;
        cin >> a >> b >> S;
        a--;
        b--;

        s = S == "imposter";
    }

    OfflineDynamicGraph odg(2 * n);
    vector<array<int, 2>> edge_id(m);
    auto link = [&](int i) {
        auto [u, v, x] = edges[i];
        edge_id[i][0] = odg.add_edge(u, v + x * n);
        edge_id[i][1] = odg.add_edge(u + n, v + (x ^ 1) * n);
    };

    auto cut = [&](int i) {
        odg.delete_edge(edge_id[i][0]);
        odg.delete_edge(edge_id[i][1]);
    };
    for (int i = t; i < m; i++) link(i);

    int j = -1;
    bool imposter = false;
    for (int i = 0; i <= m - t; i++) {
        odg.query([&, i](auto &amt) {
            if (~j) return;

            if (amt.connected(0, k - 1)) {
                j = i + 1;
                imposter = false;
            } else if (amt.connected(0, k - 1 + n)) {
                j = i + 1;
                imposter = true;
            }
        });
        if (i == m - t) break;

        link(i);
        cut(i + t);
    }
    odg.process();

    if (!~j) cout << -1;
    else cout << j << (imposter ? " imposter" : " crewmate");
}