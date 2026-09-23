#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        while (sets[v] >= 0) {
            int p = sets[v];
            if (sets[p] >= 0) sets[v] = sets[p];
            v = p;
        }
        return v;
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return false;

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        return true;
    }

    int size(int v) {
        return -sets[find(v)];
    }

    DisjointSets(int n) : sets(n, -1) {}
};

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

    void reset() {
        fill(parent.begin(), parent.end(), -1);
        fill(size.begin(), size.end(), 1);
        fill(weight.begin(), weight.end(), numeric_limits<T>::max());
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<array<int, 3>> edges(m);
    for (auto &[a, b, c] : edges) cin >> a >> b >> c;

    int e;
    cin >> e;

    vector<bool> changed(m, false);
    vector<array<int, 4>> events(e, {0, 0, 0, 0});
    for (auto &[q, a, b, w] : events) {
        char ch;
        cin >> ch >> a >> b;

        q = ch;
        if (ch == 'S') cin >> w;
        else changed[--a] = true;
    }

    vector<int> candidates, indices;
    for (int i = 0; i < m; i++)
        if (changed[i]) candidates.emplace_back(i);
        else indices.emplace_back(i);
    sort(indices.begin(), indices.end(), [&](int i, int j) { return edges[i][2] != edges[j][2] ? edges[i][2] > edges[j][2] : i < j; });

    DisjointSets dsu(n + 1);
    for (int i : indices)
        if (dsu.unite(edges[i][0], edges[i][1])) candidates.emplace_back(i);

    int k = -m - 1;
    auto weight = [&](int i) {
        return (long long) edges[i][2] * k + i;
    };

    AntiMonopolyTree<long long> amt(n + 1);
    for (int i : candidates) amt.add(edges[i][0], edges[i][1], weight(i));

    for (auto [q, a, b, w] : events) {
        if (q == 'S') {
            if (a == b) {
                cout << "1\n";
                continue;
            }

            auto [max_w, t] = amt.path_max(a, b);
            cout << (t != -1 && max_w <= (long long) w * k + m - 1) << "\n";
            continue;
        }

        auto temp = weight(a);
        auto [max_w, t] = amt.path_max(edges[a][0], edges[a][1]);
        edges[a][2] = b;
        if (q == 'R') amt.add(edges[a][0], edges[a][1], weight(a));
        else if (t != -1 && max_w == temp) {
            amt.reset();
            for (int i : candidates) amt.add(edges[i][0], edges[i][1], weight(i));
        }
    }
}
