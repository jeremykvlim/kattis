#include <bits/stdc++.h>
using namespace std;

struct Hash {
    template <typename T>
    static inline void combine(size_t &h, const T &v) {
        h ^= Hash{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }

    template <typename T>
    size_t operator()(const T &v) const {
        if constexpr (requires { tuple_size<T>::value; })
            return apply([](const auto &...e) {
                size_t h = 0;
                (combine(h, e), ...);
                return h;
            }, v);
        else if constexpr (requires { declval<T>().begin(); declval<T>().end(); } && !is_same_v<T, string>) {
            size_t h = 0;
            for (const auto &e : v) combine(h, e);
            return h;
        } else return hash<T>{}(v);
    }
};

template <typename T>
struct Point3D {
    T x, y, z;

    Point3D() {}
    Point3D(T x, T y, T z) : x(x), y(y), z(z) {}

    template <typename U>
    Point3D(U x, U y, U z) : x(x), y(y), z(z) {}

    template <typename U>
    Point3D(const Point3D<U> &p) : x((T) p.x), y((T) p.y), z((T) p.z) {}

    const auto begin() const {
        return &x;
    }

    const auto end() const {
        return &z + 1;
    }

    Point3D operator-() const {
        return {-x, -y, -z};
    }

    bool operator<(const Point3D &p) const {
        return x != p.x ? x < p.x : y != p.y ? y < p.y : z < p.z;
    }

    bool operator>(const Point3D &p) const {
        return x != p.x ? x > p.x : y != p.y ? y > p.y : z > p.z;
    }

    bool operator==(const Point3D &p) const {
        return x == p.x && y == p.y && z == p.z;
    }

    bool operator!=(const Point3D &p) const {
        return x != p.x || y != p.y || z != p.z;
    }

    bool operator<=(const Point3D &p) const {
        return *this < p || *this == p;
    }

    bool operator>=(const Point3D &p) const {
        return *this > p || *this == p;
    }

    Point3D operator+(const Point3D &p) const {
        return {x + p.x, y + p.y, z + p.z};
    }

    Point3D operator+(const T &v) const {
        return {x + v, y + v, z + v};
    }

    Point3D & operator+=(const Point3D &p) {
        x += p.x;
        y += p.y;
        z += p.z;
        return *this;
    }

    Point3D & operator+=(const T &v) {
        x += v;
        y += v;
        z += v;
        return *this;
    }

    Point3D operator-(const Point3D &p) const {
        return {x - p.x, y - p.y, z - p.z};
    }

    Point3D operator-(const T &v) const {
        return {x - v, y - v, z - v};
    }

    Point3D & operator-=(const Point3D &p) {
        x -= p.x;
        y -= p.y;
        z -= p.z;
        return *this;
    }

    Point3D & operator-=(const T &v) {
        x -= v;
        y -= v;
        z -= v;
        return *this;
    }

    Point3D operator*(const T &v) const {
        return {x * v, y * v, z * v};
    }

    Point3D & operator*=(const T &v) {
        x *= v;
        y *= v;
        z *= v;
        return *this;
    }

    Point3D operator/(const T &v) const {
        return {x / v, y / v, z / v};
    }

    Point3D & operator/=(const T &v) {
        x /= v;
        y /= v;
        z /= v;
        return *this;
    }
};

template <typename T>
double euclidean_dist(const Point3D<T> &a, const Point3D<T> &b = {0, 0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y) + (double) (a.z - b.z) * (a.z - b.z));
}

struct ImplicitTreap {
    static inline mt19937_64 rng{random_device{}()};

    struct TreapNode {
        array<int, 3> family;
        unsigned long long prio;
        int size;
        double base_A, base_B, aggregate_A, aggregate_B;

        TreapNode() : family{0, 0, 0}, prio(rng()), size(1),
                      base_A(0), base_B(0), aggregate_A(0), aggregate_B(0) {}

        auto & operator=(const pair<double, double> &v) {
            tie(base_A, base_B) = v;
            return *this;
        }
    };

    vector<TreapNode> T;

    ImplicitTreap(int n) : T(n + 1) {}

    int size(int i) const {
        return i ? T[i].size : 0;
    }

    double base_A(int i) const {
        return i ? T[i].base_A : 0;
    }

    double base_B(int i) const {
        return i ? T[i].base_B : 0;
    }

    double aggregate_A(int i) const {
        return i ? T[i].aggregate_A : 0;
    }

    double aggregate_B(int i) const {
        return i ? T[i].aggregate_B : 0;
    }

    void pull(int i) {
        if (!i) return;
        auto [l, r, p] = T[i].family;
        T[i].size = size(l) + size(r) + 1;
        T[i].aggregate_A = aggregate_A(l) + aggregate_A(r) + base_A(i);
        T[i].aggregate_B = aggregate_B(l) + aggregate_B(r) + base_B(i);
    }

    int root(int i) {
        for (; T[i].family[2]; i = T[i].family[2]);
        return i;
    }

    void attach(int i, int c, int j) {
        T[i].family[c] = j;
        if (j) T[j].family[2] = i;
        pull(i);
    }

    pair<int, int> split(int i, int k) {
        if (!i) return {0, 0};
        auto [l, r, p] = T[i].family;
        int sl = size(l);
        if (k <= sl) {
            auto [ll, lr] = split(l, k);
            attach(i, 0, lr);
            if (ll) T[ll].family[2] = 0;
            T[i].family[2] = 0;
            return {ll, i};
        } else {
            auto [rl, rr] = split(r, k - sl - 1);
            attach(i, 1, rl);
            if (rr) T[rr].family[2] = 0;
            T[i].family[2] = 0;
            return {i, rr};
        }
    }

    int meld(int i, int j) {
        if (!i || !j) {
            int k = i ^ j;
            if (k) T[k].family[2] = 0;
            return k;
        }
        if (T[i].prio < T[j].prio) {
            attach(i, 1, meld(T[i].family[1], j));
            T[i].family[2] = 0;
            return i;
        } else {
            attach(j, 0, meld(i, T[j].family[0]));
            T[j].family[2] = 0;
            return j;
        }
    }

    void point_update(int i, const pair<double, double> &v) {
        for (T[i] = v; i; i = T[i].family[2]) pull(i);
    }

    int rank(int i) {
        int r = size(T[i].family[0]);
        while (T[i].family[2]) {
            int p = T[i].family[2];
            if (T[p].family[1] == i) r += size(T[p].family[0]) + 1;
            i = p;
        }
        return r;
    }

    auto & operator[](int i) {
        return T[i];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int W, L, n, m;
        cin >> W >> L >> n >> m;

        int SW = -1, SE = -1, NW = -1, NE = -1;
        vector<Point3D<double>> coords(n);
        for (int i = 0; i < n; i++) {
            auto &[x, y, z] = coords[i];
            cin >> x >> y >> z;

            if (!x && !y) SW = i;
            if (x == W && !y) SE = i;
            if (!x && y == L) NW = i;
            if (x == W && y == L) NE = i;
        }

        int edges = 0;
        unordered_map<pair<int, int>, int, Hash> indices;
        auto edge_index = [&](int u, int v) {
            if (u > v) swap(u, v);
            auto it = indices.find({u, v});
            if (it != indices.end()) return it->second;
            return indices[{u, v}] = edges++;
        };
        int west_border = edge_index(SW, NW), east_border = edge_index(SE, NE);

        vector<int> order(n);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int i, int j) { return coords[i].z < coords[j].z; });

        vector<int> pos(n);
        for (int i = 0; i < n; i++) pos[order[i]] = i;

        vector<pair<array<int, 2>, array<double, 2>>> contours;
        vector<vector<int>> adds(n), deletes(n);
        vector<vector<pair<int, double>>> edge_lengths(n);
        while (m--) {
            int a, b, c;
            cin >> a >> b >> c;
            a--;
            b--;
            c--;

            auto add = [&](int i, int j, int k, int e) {
                auto u = coords[i] - coords[j], v = coords[k] - coords[j];
                if (u.z * (coords[k].z - coords[i].z) < 0) return;
                edge_lengths[i].emplace_back(e, euclidean_dist(coords[j] + v * (u.z / v.z), coords[i]));
            };
            add(a, b, c, edge_index(b, c));
            add(b, c, a, edge_index(c, a));
            add(c, a, b, edge_index(a, b));

            while (coords[a].z > coords[b].z || coords[a].z > coords[c].z) {
                swap(a, b);
                swap(b, c);
            }

            bool swapped = false;
            if (coords[b].z > coords[c].z) {
                swap(b, c);
                swapped = true;
            }

            auto add_contour = [&](int l, int r, int u, int v, int i, int j, int k, bool neg) {
                if (l >= r) return;

                auto slope = [&](auto &p, auto &q) {
                    auto v = q - p;
                    return Point3D{v.x, v.y, 0.} / v.z;
                };
                auto a = euclidean_dist(slope(coords[i], coords[j]) - slope(coords[i], coords[k])) * (neg ? -1 : 1), b = -coords[i].z * a;

                if (swapped) swap(u, v);
                contours.emplace_back(make_pair(array{u, v}, array{a, b}));
                adds[l].emplace_back(contours.size() - 1);
                deletes[r].emplace_back(contours.size() - 1);
            };
            int p = pos[a], q = pos[b], r = pos[c];
            add_contour(p + 1, q, edge_index(a, b), edge_index(a, c), a, b, c, false);
            add_contour(q + 1, r, edge_index(b, c), edge_index(a, c), c, a, b, true);
        }

        ImplicitTreap treap(edges);
        auto len = 1e20;
        vector<int> state(contours.size(), 0), cycle_edge(edges + 1, -1);
        for (int l = 0; l < n; l++) {
            for (int e : deletes[l]) {
                if (!state[e]) continue;

                auto [u1, v1] = contours[e].first;
                int component = treap.root(u1 + 1);
                if (state[e] == 2) {
                    if (cycle_edge[component] == e) cycle_edge[component] = -1;
                    state[e] = 0;
                } else {
                    int f = cycle_edge[component];
                    cycle_edge[component] = -1;

                    auto [tl, tr] = treap.split(component, treap.rank(u1 + 1) + 1);
                    if (tl) cycle_edge[tl] = -1;
                    if (tr) cycle_edge[tr] = -1;
                    treap.point_update(u1 + 1, {0, 0});
                    state[e] = 0;
                    if (f != -1) {
                        auto [u2, v2] = contours[f].first;
                        auto [a, b] = contours[f].second;
                        treap.point_update(u2 + 1, {a, b});
                        state[f] = 1;
                        treap.meld(tr, tl);
                    }
                }
            }

            for (int e : adds[l]) {
                if (state[e]) continue;

                auto [u, v] = contours[e].first;
                int cu = treap.root(u + 1), cv = treap.root(v + 1);
                if (cu == cv) {
                    cycle_edge[cu] = e;
                    state[e] = 2;
                } else {
                    auto [a, b] = contours[e].second;
                    treap.point_update(u + 1, {a, b});
                    treap.meld(cu, cv);
                    state[e] = 1;
                }
            }

            int i = order[l];
            double west_len = (i == SW || i == NW) ? 0 : 1e20, east_len = (i == SE || i == NE) ? 0 : 1e20,
                   west_base = -1, east_base = -1;
            int west = treap.root(west_border + 1), east = treap.root(east_border + 1);
            for (auto [e, d] : edge_lengths[i]) {
                int component = treap.root(e + 1);
                auto relax = [&](auto &length, auto &base, int border) {
                    if (e == border) {
                        length = min(length, d);
                        return;
                    }
                    if (base < 0) {
                        auto A = treap[component].aggregate_A, B = treap[component].aggregate_B;
                        int f = cycle_edge[component];
                        if (f != -1) {
                            auto [a, b] = contours[f].second;
                            A += a;
                            B += b;
                        }
                        base = A * coords[i].z + B;
                    }
                    length = min(length, base + d);
                };
                if (component == west) relax(west_len, west_base, west_border);
                if (component == east) relax(east_len, east_base, east_border);
            }

            if (west_len != 1e20 && east_len != 1e20) len = min(len, west_len + east_len);
        }

        if (len == 1e20) cout << "impossible\n";
        else cout << fixed << setprecision(6) << len << "\n";
    }
}
