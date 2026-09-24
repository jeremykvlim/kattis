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

        int nodes = 0;
        unordered_map<pair<int, int>, int, Hash> indices;
        auto node_id = [&](int u, int v) {
            auto [it, inserted] = indices.try_emplace(minmax(u, v), nodes);
            if (inserted) nodes++;
            return it->second;
        };
        int west_border = node_id(SW, NW), east_border = node_id(SE, NE);

        vector<int> order(n);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int i, int j) { return coords[i].z < coords[j].z; });

        vector<int> pos(n);
        for (int i = 0; i < n; i++) pos[order[i]] = i;

        vector<pair<array<int, 2>, complex<double>>> contours;
        vector<vector<int>> adds(n), deletes(n);
        vector<vector<pair<int, double>>> node_lengths(n);
        while (m--) {
            int a, b, c;
            cin >> a >> b >> c;
            a--;
            b--;
            c--;

            auto add = [&](int i, int j, int k, int t) {
                auto u = coords[i] - coords[j], v = coords[k] - coords[j];
                if (u.z * (coords[k].z - coords[i].z) < 0) return;
                node_lengths[i].emplace_back(t, euclidean_dist(coords[j] + v * (u.z / v.z), coords[i]));
            };
            add(a, b, c, node_id(b, c));
            add(b, c, a, node_id(c, a));
            add(c, a, b, node_id(a, b));

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
                contours.emplace_back(make_pair(array{u, v}, complex{a, b}));
                adds[l].emplace_back(contours.size() - 1);
                deletes[r].emplace_back(contours.size() - 1);
            };
            int p = pos[a], q = pos[b], r = pos[c];
            add_contour(p + 1, q, node_id(a, b), node_id(a, c), a, b, c, false);
            add_contour(q + 1, r, node_id(b, c), node_id(a, c), c, a, b, true);
        }

        vector<complex<double>> c(contours.size() + nodes);
        for (int e = 0; e < contours.size(); e++) c[e + nodes] = contours[e].second;

        auto len = 1e20;
        OfflineDynamicGraph<complex<double>> odg(c);
        vector<array<int, 2>> edge_id(contours.size());
        for (int l = 0; l < n; l++) {
            for (int e : deletes[l]) {
                odg.delete_edge(edge_id[e][0]);
                odg.delete_edge(edge_id[e][1]);
            }

            for (int e : adds[l]) {
                auto [u, v] = contours[e].first;
                edge_id[e][0] = odg.add_edge(u, e + nodes);
                edge_id[e][1] = odg.add_edge(e + nodes, v);
            }

            odg.query([&, l](auto &amt) {
                int i = order[l];
                double west_len = (i == SW || i == NW) ? 0 : 1e20, east_len = (i == SE || i == NE) ? 0 : 1e20,
                       west_base = -1, east_base = -1;

                int west = amt.root(west_border), east = amt.root(east_border);
                for (auto [v, d] : node_lengths[i]) {
                    int component = amt.root(v);
                    auto relax = [&](double &length, double &base, int border) {
                        if (v == border) {
                            length = min(length, d);
                            return;
                        }

                        if (base < 0) {
                            auto sum = amt.sum[component];
                            base = sum.real() * coords[i].z + sum.imag();
                        }
                        length = min(length, base + d);
                    };
                    if (component == west) relax(west_len, west_base, west_border);
                    if (component == east) relax(east_len, east_base, east_border);
                }

                if (west_len != 1e20 && east_len != 1e20) len = min(len, west_len + east_len);
            });
        }
        odg.process();

        if (len == 1e20) cout << "impossible\n";
        else cout << fixed << setprecision(6) << len << "\n";
    }
}
