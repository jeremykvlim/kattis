#include <bits/stdc++.h>
using namespace std;

struct SplayTree {
    struct SplayNode {
        array<int, 3> family;
        bool flip;
        int value;

        SplayNode() : family{0, 0, 0}, flip(false), value(-1) {}
    };

    vector<SplayNode> ST;

    SplayTree(int n, int m) : ST(n + m + 1) {
        for (int i = n + 1; i <= n + m; i++) ST[i].value = i - (n + 1);
    }

    auto & operator[](int i) {
        return ST[i];
    }

    void pull(int i) {
        if (!i) return;
    }

    void flip(int i) {
        if (!i) return;
        auto &[l, r, p] = ST[i].family;
        swap(l, r);
        ST[i].flip ^= true;
    }

    void push(int i) {
        if (!i) return;
        if (ST[i].flip) {
            auto [l, r, p] = ST[i].family;
            if (l) flip(l);
            if (r) flip(r);
            ST[i].flip = false;
        }
    }

    void splay(int i) {
        auto root = [&](int i) {
            auto [l, r, p] = ST[ST[i].family[2]].family;
            return !i || l != i && r != i;
        };

        auto child = [&](int i, int parent) { return ST[parent].family[1] == i; };

        auto rotate = [&](int i) {
            int j = ST[i].family[2], k = ST[j].family[2];
            if (!root(j)) ST[k].family[child(j, k)] = i;

            int c = child(i, j), s = ST[j].family[c] = ST[i].family[c ^ 1];
            if (s) ST[s].family[2] = j;

            ST[i].family[c ^ 1] = j;
            ST[i].family[2] = k;
            ST[j].family[2] = i;
            pull(j);
        };

        auto propagate = [&](auto &&self, int i) -> void {
            if (!root(i)) self(self, ST[i].family[2]);
            push(i);
        };

        propagate(propagate, i);
        while (!root(i)) {
            int j = ST[i].family[2], k = ST[j].family[2];
            if (!root(j)) rotate(child(i, j) != child(j, k) ? i : j);
            rotate(i);
        }
        pull(i);
    }
};

struct LinkCutTree : SplayTree {
    LinkCutTree(int n, int m) : SplayTree(n, m) {}

    void access(int i) {
        for (int u = 0, v = i; v; u = v, v = ST[v].family[2]) {
            splay(v);
            ST[v].family[1] = u;
            pull(v);
        }
        splay(i);
    }

    void reroot(int i) {
        access(i);
        flip(i);
        pull(i);
    }

    void link(int i, int j) {
        reroot(i);
        ST[i].family[2] = j;
    }

    void split(int i, int j) {
        reroot(j);
        access(i);
    }

    void cut(int i, int j) {
        split(i, j);
        ST[i].family[0] = ST[j].family[2] = 0;
        pull(i);
    }

    vector<int> path(int i, int j) {
        split(i, j);
        vector<int> path;
        auto dfs = [&](auto &&self, int v) -> void {
            if (!v) return;
            push(v);
            self(self, ST[v].family[0]);
            if (ST[v].value != -1) path.emplace_back(ST[v].value);
            self(self, ST[v].family[1]);
        };
        dfs(dfs, i);
        return path;
    }
};

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

pair<vector<int>, vector<bool>> kruskal(int n, vector<array<int, 4>> edges) {
    DisjointSets dsu(n);
    sort(edges.begin(), edges.end());

    vector<int> mst;
    vector<bool> in_mst(edges.size(), false);
    for (auto [w, u, v, i] : edges)
        if (dsu.unite(u, v)) {
            mst.emplace_back(i);
            in_mst[i] = true;
            if (mst.size() == n - 1) break;
        }

    return {mst, in_mst};
}
template <typename T>
vector<complex<T>> quadratic_roots(T a, T b, T c) {
    if (fabs(a) < 1e-8 && fabs(b) < 1e-8) return {};
    if (fabs(a) < 1e-8) return {(-c / b)};

    complex<T> discriminant(b * b - 4 * a * c, 0);
    return {(-b + sqrt(discriminant)) / (2 * a), (-b - sqrt(discriminant)) / (2 * a)};
}

template <typename T>
int kinetic_euclidean_minimum_spanning_tree(int n, const vector<array<T, 6>> &points) {
    int m = n * (n - 1) / 2;
    vector<pair<int, int>> edges(m);
    for (int i = 0, k = 0; i < n; i++)
        for (int j = i + 1; j < n; j++, k++) edges[k] = {i, j};

    vector<T> A(m), B(m), C(m);
    for (int k = 0; k < m; k++) {
        auto [i, j] = edges[k];
        T dx = points[i][0] - points[j][0], dy = points[i][1] - points[j][1], dz = points[i][2] - points[j][2],
          dvx = points[i][3] - points[j][3], dvy = points[i][4] - points[j][4], dvz = points[i][5] - points[j][5];

        A[k] = dvx * dvx + dvy * dvy + dvz * dvz;
        B[k] = 2 * (dx * dvx + dy * dvy + dz * dvz);
        C[k] = dx * dx + dy * dy + dz * dz;
    }

    auto f = [&](double t, int k) {
        return A[k] * t * t + B[k] * t + C[k];
    };

    auto lower_weight_edge = [&](double t, int non_mst_edge, const vector<int> &path) {
        auto weight = -1e20;
        for (int mst_edge : path) weight = max(weight, f(t, mst_edge));
        return f(t, non_mst_edge) + 1e-8 < weight;
    };

    vector<pair<int, int>> order(m);
    for (int k = 0; k < m; k++) order[k] = {C[k], k};
    sort(order.begin(), order.end());

    vector<array<int, 4>> temp(m);
    for (int i = 0; i < m; i++) {
        auto [u, v] = edges[order[i].second];
        temp[i] = {i, u, v, order[i].second};
    }
    auto [mst, in_mst] = kruskal(n, temp);

    LinkCutTree lct(n, m);
    for (int mst_edge : mst) {
        auto [u, v] = edges[mst_edge];
        lct.link(mst_edge + n + 1, u + 1);
        lct.link(mst_edge + n + 1, v + 1);
    }

    priority_queue<tuple<double, int, int>, vector<tuple<double, int, int>>, greater<>> pq;
    vector<vector<int>> candidates(m);
    vector<int> version(m, 0), replace(m, -1), pos(m, -1);
    auto dequeue = [&](int non_mst_edge) {
        version[non_mst_edge]++;

        int mst_edge = replace[non_mst_edge];
        if (mst_edge != -1) {
            int p = pos[non_mst_edge];

            auto remove = [&]() {
                swap(candidates[mst_edge][p], candidates[mst_edge].back());
                candidates[mst_edge].pop_back();
                pos[candidates[mst_edge][p]] = p;
            };

            if (p != -1 && p < candidates[mst_edge].size() && candidates[mst_edge][p] == non_mst_edge) remove();
            else
                for (p = 0; p < candidates[mst_edge].size(); p++)
                    if (candidates[mst_edge][p] == non_mst_edge) {
                        remove();
                        break;
                    }

            replace[non_mst_edge] = pos[non_mst_edge] = -1;
        }
    };

    auto enqueue = [&](int non_mst_edge, double r = 0) {
        version[non_mst_edge]++;

        auto edge_to_replace = [&]() -> pair<double, int> {
            auto [u, v] = edges[non_mst_edge];
            auto path = lct.path(u + 1, v + 1);

            vector<pair<double, int>> same_weight_edges;
            for (int mst_edge : path) {
                double a = A[non_mst_edge] - A[mst_edge], b = B[non_mst_edge] - B[mst_edge], c = C[non_mst_edge] - C[mst_edge];
                for (auto root : quadratic_roots(a, b, c))
                    if (root.imag() < 1e-8 && root.real() - r > 1e-8) same_weight_edges.emplace_back(root.real(), mst_edge);
            }
            if (same_weight_edges.empty()) return {1e20, -1};
            sort(same_weight_edges.begin(), same_weight_edges.end());
            same_weight_edges.erase(unique(same_weight_edges.begin(), same_weight_edges.end()), same_weight_edges.end());

            for (auto [t1, e] : same_weight_edges)
                if (fabs(f(t1, non_mst_edge) - f(t1, e)) <= 1e-6)
                    if (lower_weight_edge(t1 + 1e-6, non_mst_edge, path)) return {t1, e};

            return {1e20, -1};
        };

        auto [t, mst_edge] = edge_to_replace();
        if (mst_edge != -1) {
            candidates[mst_edge].emplace_back(non_mst_edge);
            pos[non_mst_edge] = candidates[mst_edge].size() - 1;
            replace[non_mst_edge] = mst_edge;
            pq.emplace(t, version[non_mst_edge], non_mst_edge);
        }
    };

    for (int k = 0; k < m; k++)
        if (!in_mst[k]) enqueue(k);

    double t = 0;
    int modifications = 1;
    while (!pq.empty()) {
        auto [t1, ver, non_mst_edge] = pq.top();
        pq.pop();

        if (ver != version[non_mst_edge] || in_mst[non_mst_edge]) continue;

        auto [u, v] = edges[non_mst_edge];
        auto path = lct.path(u + 1, v + 1);

        auto weight = -1e20;
        int mst_edge = -1;
        for (int e : path) {
            auto w = f(t1, e);
            if (weight < w) {
                weight = w;
                mst_edge = e;
            }
        }

        if (fabs(f(t1, non_mst_edge) - weight) <= 1e-6)
            if (lower_weight_edge(t1 + 1e-6, non_mst_edge, path)) {
                modifications++;
                swap(non_mst_edge, mst_edge);
                auto [i, j] = edges[non_mst_edge];
                dequeue(mst_edge);
                lct.cut(non_mst_edge + n + 1, i + 1);
                lct.cut(non_mst_edge + n + 1, j + 1);
                in_mst[non_mst_edge] = false;
                lct.link(mst_edge + n + 1, u + 1);
                lct.link(mst_edge + n + 1, v + 1);
                in_mst[mst_edge] = true;

                t = t1 + 1e-6;
                for (int e : candidates[non_mst_edge]) enqueue(e, t);
                enqueue(non_mst_edge, t);
                continue;
            }
        enqueue(mst_edge, max(t, t1 + 1e-6));
    }

    return modifications;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int d;
    while (cin >> d) {
        vector<array<int, 6>> points(d);
        for (auto &[x, y, z, vx, vy, vz] : points) cin >> x >> y >> z >> vx >> vy >> vz;
        cout << kinetic_euclidean_minimum_spanning_tree(d, points) << "\n";
    }
}
