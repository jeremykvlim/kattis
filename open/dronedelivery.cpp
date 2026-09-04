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
int kinetic_euclidean_mst(int n, const vector<array<T, 6>> &points) {
    int m = n * (n - 1) / 2;
    vector<pair<int, int>> edges(m);
    vector<T> A(m), B(m), C(m);
    for (int i = 0, k = 0; i < n; i++)
        for (int j = i + 1; j < n; j++, k++) {
            edges[k] = {i, j};
            T dx = points[i][0] - points[j][0], dy = points[i][1] - points[j][1], dz = points[i][2] - points[j][2],
              dvx = points[i][3] - points[j][3], dvy = points[i][4] - points[j][4], dvz = points[i][5] - points[j][5];

            A[k] = dvx * dvx + dvy * dvy + dvz * dvz;
            B[k] = 2 * (dx * dvx + dy * dvy + dz * dvz);
            C[k] = dx * dx + dy * dy + dz * dz;
        }

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
    for (int e : mst) {
        auto [u, v] = edges[e];
        lct.link(n + e + 1, u + 1);
        lct.link(n + e + 1, v + 1);
    }

    priority_queue<tuple<double, int, int>, vector<tuple<double, int, int>>, greater<>> pq;
    vector<vector<int>> candidates(m);
    vector<int> version(m, 0), replace(m, -1), pos(m, -1);
    auto unschedule = [&](int non_mst_edge) {
        version[non_mst_edge]++;

        int mst_edge = replace[non_mst_edge];
        if (~mst_edge) {
            int p = pos[non_mst_edge];
            swap(candidates[mst_edge][p], candidates[mst_edge].back());
            pos[candidates[mst_edge][p]] = p;
            candidates[mst_edge].pop_back();
            replace[non_mst_edge] = pos[non_mst_edge] = -1;
        }
    };

    auto schedule = [&](int non_mst_edge, double r = 0) {
        unschedule(non_mst_edge);

        pair<double, int> edge_to_replace{1e20, -1};
        auto [u, v] = edges[non_mst_edge];
        for (int mst_edge : lct.path(u + 1, v + 1)) {
            double a = A[non_mst_edge] - A[mst_edge], b = B[non_mst_edge] - B[mst_edge], c = C[non_mst_edge] - C[mst_edge];
            for (auto root : quadratic_roots(a, b, c))
                if (fabs(root.imag()) <= 1e-8) {
                    auto t = root.real();
                    if (r + 1e-8 < t && t < edge_to_replace.first && 2 * a * t + b < 0) edge_to_replace = {t, mst_edge};
                }
        }
        if (edge_to_replace == make_pair(1e20, -1)) return;

        auto [t, mst_edge] = edge_to_replace;
        candidates[mst_edge].emplace_back(non_mst_edge);
        pos[non_mst_edge] = candidates[mst_edge].size() - 1;
        replace[non_mst_edge] = mst_edge;
        pq.emplace(t, version[non_mst_edge], non_mst_edge);
    };
    for (int e = 0; e < m; e++)
        if (!in_mst[e]) schedule(e);

    int modifications = 1;
    while (!pq.empty()) {
        auto [t, ver, adding] = pq.top();
        pq.pop();

        if (ver != version[adding] || in_mst[adding]) continue;

        int removing = replace[adding];
        auto [u, v] = edges[adding];
        auto path = lct.path(u + 1, v + 1);
        if (find(path.begin(), path.end(), removing) == path.end()) {
            schedule(adding, t);
            continue;
        }

        modifications++;
        unschedule(adding);
        auto [i, j] = edges[removing];
        lct.cut(n + removing + 1, i + 1);
        lct.cut(n + removing + 1, j + 1);
        in_mst[removing] = false;
        lct.link(n + adding + 1, u + 1);
        lct.link(n + adding + 1, v + 1);
        in_mst[adding] = true;
        for (int e : candidates[removing]) replace[e] = pos[e] = -1;
        for (int e : candidates[removing]) schedule(e, t);
        candidates[removing].clear();
        schedule(removing, t);
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
        cout << kinetic_euclidean_mst(d, points) << "\n";
    }
}
