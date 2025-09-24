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
bool approximately_equal(const T &v1, const T &v2, double epsilon = 1e-5) {
    return fabs(v1 - v2) <= epsilon;
}

template <typename T>
int sgn(const T &v) {
    if (!is_floating_point_v<T>) return (v > 0) - (v < 0);
    return approximately_equal(v, (T) 0) ? 0 : (v > 0) - (v < 0);
}

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    template <typename U>
    Point(U x, U y) : x(x), y(y) {}

    template <typename U>
    Point(const Point<U> &p) : x((T) p.x), y((T) p.y) {}

    const auto begin() const {
        return &x;
    }

    const auto end() const {
        return &y + 1;
    }

    Point operator-() const {
        return {-x, -y};
    }

    bool operator<(const Point &p) const {
        return x != p.x ? x < p.x : y < p.y;
    }

    bool operator>(const Point &p) const {
        return x != p.x ? x > p.x : y > p.y;
    }

    bool operator==(const Point &p) const {
        return x == p.x && y == p.y;
    }

    bool operator!=(const Point &p) const {
        return x != p.x || y != p.y;
    }

    bool operator<=(const Point &p) const {
        return *this < p || *this == p;
    }

    bool operator>=(const Point &p) const {
        return *this > p || *this == p;
    }

    Point operator+(const Point &p) const {
        return {x + p.x, y + p.y};
    }

    Point operator+(const T &v) const {
        return {x + v, y + v};
    }

    Point & operator+=(const Point &p) {
        x += p.x;
        y += p.y;
        return *this;
    }

    Point & operator+=(const T &v) {
        x += v;
        y += v;
        return *this;
    }

    Point operator-(const Point &p) const {
        return {x - p.x, y - p.y};
    }

    Point operator-(const T &v) const {
        return {x - v, y - v};
    }

    Point & operator-=(const Point &p) {
        x -= p.x;
        y -= p.y;
        return *this;
    }

    Point & operator-=(const T &v) {
        x -= v;
        y -= v;
        return *this;
    }

    Point operator*(const T &v) const {
        return {x * v, y * v};
    }

    Point & operator*=(const T &v) {
        x *= v;
        y *= v;
        return *this;
    }

    Point operator/(const T &v) const {
        return {x / v, y / v};
    }

    Point & operator/=(const T &v) {
        x /= v;
        y /= v;
        return *this;
    }
};

template <typename T>
T dot(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.x) + (a.y * b.y);
}

template <typename T>
T cross(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.y) - (a.y * b.x);
}

template <typename T>
T cross(const Point<T> &a, const Point<T> &b, const Point<T> &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

template <typename T>
double euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

template <typename T>
T squared_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

template <typename T, typename W>
Point<T> circumcenter(const array<pair<Point<T>, int>, 3> &triangle, W &&weight) {
    auto [a, b, c] = triangle;
    Point<T> ab = a.first - b.first, bc = b.first - c.first, ca = c.first - a.first;

    T A = squared_dist(a.first) - weight(a), B = squared_dist(b.first) - weight(b), C = squared_dist(c.first) - weight(c), d = 2 * cross(a.first, b.first, c.first);
    return {(A * bc.y + B * ca.y + C * ab.y) / d, (A * (-bc.x) + B * (-ca.x) + C * (-ab.x)) / d};
}

template <typename T, typename W>
pair<bool, bool> point_in_circumcircle(const array<pair<Point<T>, int>, 3> &triangle, const pair<Point<T>, int> &p, W &&weight) {
    auto [a, b, c] = triangle;
    T det = cross(a.first, b.first, p.first) * weight(c, p) + cross(b.first, c.first, p.first) * weight(a, p) + cross(c.first, a.first, p.first) * weight(b, p);
    if (sgn(det) >= 0) return {sgn(det) > 0, !sgn(det)};
    return {false, false};
}

template <typename T>
pair<bool, bool> point_in_polygon(const vector<Point<T>> &polygon, const Point<T> &p) {
    bool in = false;
    for (int i = 0; i < polygon.size(); i++) {
        auto a = polygon[i] - p, b = polygon[(i + 1) % polygon.size()] - p;
        if (a.y > b.y) swap(a, b);
        if (sgn(a.y) <= 0 && 0 < sgn(b.y) && sgn(cross(a, b)) < 0) in = !in;
        if (!sgn(cross(a, b)) && sgn(dot(a, b)) <= 0) return {false, true};
    }
    return {in, false};
}

template <typename T>
struct Line {
    Point<T> a, b;

    Line() {}
    Line(Point<T> a, Point<T> b) : a(a), b(b) {}
};

template <typename T>
Line<T> power_bisector(const Point<T> &a, const Point<T> &b, T za, T zb) {
    auto v = b - a, p = a + v * (((zb - za) / 2 - dot(a, v)) / dot(v, v));
    return {p, {p.x - v.y, p.y + v.x}};
}

template <typename... I>
auto hilbert_index(I... c) {
    using T = common_type_t<I...>;
    constexpr int D = sizeof...(I);
    array<T, D> coords{(T) c...};
    T c_max = max({c...});
    int b = 0;
    if (c_max)
        while (c_max >>= 1) b++;

    for (T mask = ((T) 1) << b; mask > 1; mask >>= 1)
        for (int i = D - 1; ~i; i--)
            if (coords[i] & mask) coords[0] ^= mask - 1;
            else {
                T m = (coords[0] ^ coords[i]) & (mask - 1);
                coords[0] ^= m;
                coords[i] ^= m;
            }

    for (int i = 1; i < D; i++) coords[i] ^= coords[i - 1];
    T m = 0;
    for (T mask = ((T) 1) << b; mask > 1; mask >>= 1)
        if (coords[D - 1] & mask) m ^= mask - 1;
    for (int i = 0; i < D; i++) coords[i] ^= m;

    T h = 0;
    for (; ~b; b--)
        for (int i = 0; i < D; i++) h = (h << 1) | ((coords[i] >> b) & 1);
    return h;
}

template <typename T>
struct PowerTriangulation {
    using U = typename conditional<(is_same<T, int>::value || is_same<T, long long>::value), double, typename conditional<is_same<T, __int128>::value, long double, void>::type>::type;

    struct Triangle {
        int a, b, c;
        bool valid;

        Triangle() : a(-1), b(-1), c(-1), valid(false) {}
        Triangle(int a, int b, int c, bool valid) : a(a), b(b), c(c), valid(valid) {}
    };

    vector<Point<T>> points;
    vector<T> weights, z;
    vector<Triangle> triangles;
    vector<array<int, 3>> adj_list;
    vector<pair<int, int>> delaunay_edges;
    vector<Point<U>> power_vertices;
    vector<Line<U>> power_edges;
    vector<int> vertex_match, edge_match;

    PowerTriangulation(vector<Point<T>> p, vector<T> w) {
        int n = p.size();

        T xl = p[0].x, xr = p[0].x, yl = p[0].y, yr = p[0].y;
        for (auto [x, y] : p) {
            xl = min(xl, x);
            xr = max(xr, x);
            yl = min(yl, y);
            yr = max(yr, y);
        }

        T delta = 1000 * max(xr - xl, yr - yl), xm = xl + (xr - xl) / 2, ym = yl + (yr - yl) / 2;
        p.emplace_back(xm + delta, ym - delta);
        p.emplace_back(xm, ym + delta);
        p.emplace_back(xm - delta, ym);
        w.emplace_back(0);
        w.emplace_back(0);
        w.emplace_back(0);

        z.resize(n + 3);
        for (int i = 0; i < n + 3; i++) {
            auto [x, y] = p[i];
            z[i] = x * x + y * y - w[i];
        }

        int a = n, b = n + 1, c = n + 2;
        if (sgn(cross(p[a], p[b], p[c])) == -1) swap(b, c);
        triangles.emplace_back(a, b, c, true);
        adj_list.push_back({-1, -1, -1});

        vector<int> order(n);
        iota(order.begin(), order.end(), 0);

        vector<T> hilbert_order(n);
        for (int i = 0; i < n; i++) hilbert_order[i] = hilbert_index(p[i].x - xl, p[i].y - yl);
        sort(order.begin(), order.end(), [&](int i, int j) { return hilbert_order[i] < hilbert_order[j]; });

        points = p;
        weights = w;
        bowyer_watson(order);

        int count = 0;
        vector<int> indices(triangles.size(), -1);
        for (int i = 0; i < triangles.size(); i++) {
            if (!triangles[i].valid) continue;
            for (int e = 0; e < 3; e++) {
                int j = adj_list[i][e];
                if (j != -1 && i >= j) continue;
                auto [u, v] = triangle_edge(i, e);
                if (u < n && v < n) delaunay_edges.emplace_back(u, v);
            }
            if (triangles[i].a < n && triangles[i].b < n && triangles[i].c < n) indices[i] = count++;
        }

        vector<Triangle> temp_t(count);
        vector<array<int, 3>> temp_a(count, {-1, -1, -1});
        for (int i = 0, k = 0; i < triangles.size() && k < count; i++)
            if (indices[i] != -1) {
                for (int e = 0; e < 3; e++) {
                    int j = adj_list[i][e];
                    if (0 <= j && j < triangles.size()) temp_a[k][e] = indices[j];
                }
                temp_t[k++] = triangles[i];
            }
        triangles = temp_t;
        adj_list = temp_a;
    }

    void bowyer_watson(const vector<int> &order) {
        int n = points.size(), k = 0;
        vector<int> next(n, -1), visited_t(1, -1), visited_v1(n, -1), visited_v2(n, -1), indices(n, -1);
        for (int p : order) {
            if (visited_t.size() < triangles.size()) visited_t.resize(triangles.size(), -1);

            if (!triangles.empty())
                for (int _ = 0; _ <= triangles.size(); _++) {
                    auto [a, b, c, valid] = triangles[k];
                    if (!valid) {
                        k = 0;
                        continue;
                    }

                    int s1 = sgn(cross(points[a], points[b], points[p])), s2 = sgn(cross(points[b], points[c], points[p])), s3 = sgn(cross(points[c], points[a], points[p]));
                    if ((s1 >= 0 && s2 >= 0 && s3 >= 0) || (s1 <= 0 && s2 <= 0 && s3 <= 0)) break;

                    if (s1 == -1) {
                        if (adj_list[k][2] == -1) break;
                        k = adj_list[k][2];
                    } else if (s2 == -1) {
                        if (adj_list[k][0] == -1) break;
                        k = adj_list[k][0];
                    } else {
                        if (adj_list[k][1] == -1) break;
                        k = adj_list[k][1];
                    }
                }

            vector<int> cavity;
            queue<int> q;
            q.emplace(k);
            while (!q.empty()) {
                int i = q.front();
                q.pop();

                auto in_circle = [&](int a, int b, int c, int d) {
                    return point_in_circumcircle<U>({{{points[a], a}, {points[b], b}, {points[c], c}}}, {points[d], d},
                                                    [&](auto p1, auto p2) { return z[p1.second] - z[p2.second]; }).first;
                };

                auto [a, b, c, valid] = triangles[i];
                if (!(0 <= i && i < triangles.size()) || !valid || visited_t[i] == p || !in_circle(a, b, c, p)) continue;

                visited_t[i] = p;
                cavity.emplace_back(i);
                for (int j : adj_list[i]) {
                    if (j != -1 && triangles[j].valid && visited_t[j] != p) q.emplace(j);
                    if (j != -1 && visited_t[j] == p) q.emplace(j);
                }
            }
            if (cavity.empty()) continue;

            vector<pair<int, int>> boundary_edges, boundary_triangles;
            for (int i : cavity)
                for (int e = 0; e < 3; e++) {
                    int j = adj_list[i][e];
                    if (j != -1 && visited_t[j] == p) continue;

                    auto [u, v] = triangle_edge(i, e);
                    boundary_edges.emplace_back(u, v);

                    if (j != -1) {
                        for (int f = 0; f < 3; f++)
                            if (adj_list[j][f] == i) {
                                boundary_triangles.emplace_back(j, f);
                                break;
                            }
                    } else boundary_triangles.emplace_back(-1, -1);

                    if (visited_v1[u] != p) {
                        visited_v1[u] = p;
                        next[u] = -1;
                    }
                    if (visited_v1[v] != p) {
                        visited_v1[v] = p;
                        next[v] = -1;
                    }

                    next[u] = v;
                    indices[u] = boundary_edges.size() - 1;
                    q.emplace(u);
                }

            while (!q.empty()) {
                int s = q.front();
                q.pop();

                if (visited_v1[s] != p || visited_v2[s] == p) continue;

                int t = s, j = -1, l = -1, prev_vp = -1, prev_pu = -1;
                do {
                    visited_v2[t] = p;
                    auto [u, v] = boundary_edges[indices[t]];

                    auto edge_id = [&](const auto &triangle, const auto &edge) {
                        auto [a, b, c, _] = triangle;
                        if (minmax(b, c) == edge) return 0;
                        if (minmax(c, a) == edge) return 1;
                        if (minmax(a, b) == edge) return 2;
                        return -1;
                    };

                    int a = u, b = v, c = p;
                    if (sgn(cross(points[a], points[b], points[c])) == -1) swap(b, c);
                    triangles.emplace_back(a, b, c, true);
                    adj_list.push_back({-1, -1, -1});

                    int uv = edge_id(triangles.back(), minmax(u, v)), vp = edge_id(triangles.back(), minmax(v, p)), pu = edge_id(triangles.back(), minmax(p, u));
                    k = triangles.size() - 1;
                    auto [i, e] = boundary_triangles[indices[t]];

                    if (i != -1 && visited_t[i] != p && triangles[i].valid && edge_id(triangles[i], minmax(u, v)) != -1) {
                        adj_list[k][uv] = i;
                        if (e != -1) adj_list[i][e] = k;
                    } else adj_list[k][uv] = -1;

                    if (j != -1) {
                        adj_list[j][prev_vp] = k;
                        adj_list[k][pu] = j;
                    } else {
                        l = k;
                        prev_pu = pu;
                    }

                    j = k;
                    prev_vp = vp;
                    t = next[t];
                } while (t != s && t != -1);

                if (l != -1 && j != -1) {
                    adj_list[j][prev_vp] = l;
                    adj_list[l][prev_pu] = j;
                }
            }

            for (int i : cavity) triangles[i].valid = false;
        }
    }

    void build_power_diagram() {
        int t = triangles.size();

        vector<int> ids(t);
        unordered_map<Point<U>, int, Hash> indices;
        auto add_vertex = [&](auto p) -> int {
            if (indices.count(p)) return indices[p];
            power_vertices.emplace_back(p);
            vertex_match.emplace_back();
            return indices[p] = power_vertices.size() - 1;
        };

        for (int i = 0; i < t; i++) {
            auto [a, b, c, _] = triangles[i];
            ids[i] = add_vertex(circumcenter<U>({{{points[a], a}, {points[b], b}, {points[c], c}}}, [&](auto p) { return weights[p.second]; }));
        }

        unordered_set<pair<int, int>, Hash> seen;
        auto add_edge = [&](int i, int j, int k) {
            auto [p, q] = minmax(i, j);
            vertex_match[p] = vertex_match[q] = k;
            if (!seen.count({p, q})) {
                seen.emplace(p, q);
                power_edges.emplace_back(power_vertices[p], power_vertices[q]);
                edge_match.emplace_back(k);
            }
        };

        for (int i = 0; i < t; i++)
            for (int e = 0; e < 3; e++) {
                auto [u, v] = triangle_edge(i, e);
                int j = adj_list[i][e];
                if (j != -1) add_edge(ids[i], ids[j], min(u, v));
                else {
                    auto [a, b] = power_bisector<U>(points[u], points[v], z[u], z[v]);
                    auto dir = b - a;
                    int p = !e ? triangles[i].a : (e == 1 ? triangles[i].b : triangles[i].c);
                    add_edge(ids[i], sgn(dot((Point<U>) points[p] - a, dir)) == 1 ? add_vertex(a - (dir / 2)) : add_vertex(a + (dir / 2)), min(u, v));
                }
            }
    }

    pair<int, int> triangle_edge(int i, int e) {
        if (!e) return {triangles[i].b, triangles[i].c};
        if (e == 1) return {triangles[i].c, triangles[i].a};
        return {triangles[i].a, triangles[i].b};
    }

    array<int, 3> triangle_vertices(int i) {
        return {triangles[i].a, triangles[i].b, triangles[i].c};
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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long h;
    Point<long long> start, end;
    cin >> n >> h >> start.x >> start.y >> end.x >> end.y;

    vector<Point<long long>> coords(n);
    for (auto &[x, y] : coords) cin >> x >> y;

    int m;
    cin >> m;

    int size = 0;
    vector<int> J(m), id(m);
    vector<long long> R(m), R_max(n, 0);
    for (int i = 0; i < m; i++) {
        int ji;
        long long ri;
        cin >> ji >> ri;
        ji--;

        if (R_max[ji] < ri) {
            R_max[ji] = ri;
            J[size] = ji;
            R[size] = ri;
            id[size] = i + 1;
            size++;
        }
    }
    m = size;

    vector<long long> radius(n);
    int l = 0, r = m + 1, mid;
    while (l + 1 < r) {
        mid = l + (r - l) / 2;

        fill(radius.begin(), radius.end(), 0);
        for (int i = 0; i < mid; i++) radius[J[i]] = R[i];

        bool blocked = false;
        vector<int> active;
        for (int i = 0; i < n; i++)
            if (radius[i]) {
                active.emplace_back(i);

                if (radius[i] > h)
                    if (euclidean_dist(coords[i], start) < radius[i] - h ||
                        euclidean_dist(coords[i], end) < radius[i] - h) {
                        blocked = true;
                        break;
                    }
            }

        if (blocked) r = mid;
        else if (active.size() < 3) l = mid;
        else {
            vector<Point<__int128>> points;
            vector<__int128> weights;
            for (int i : active) {
                points.emplace_back(coords[i]);
                weights.emplace_back(radius[i] * radius[i] - h * h);
            }

            PowerTriangulation pt(points, weights);
            int s = pt.triangles.size();
            DisjointSets dsu(s + 1);
            for (int i = 0; i < s; i++) {
                array<pair<int, int>, 3> edges{{pt.triangle_edge(i, 0), pt.triangle_edge(i, 1), pt.triangle_edge(i, 2)}};
                for (int e = 0; e < 3; e++) {
                    auto [u, v] = edges[e];
                    int p = active[u], q = active[v], j = pt.adj_list[i][e];
                    auto rp = radius[p], rq = radius[q];
                    if (!(rp > h && rq > h) || euclidean_dist(coords[p], coords[q]) >= sqrt(rp * rp - h * h) + sqrt(rq * rq - h * h)) dsu.unite(i, j == -1 ? s : j);
                }
            }

            int start_face = s, end_face = s;
            for (int i = 0; i < s; i++) {
                auto [a, b, c] = pt.triangle_vertices(i);
                vector<Point<long long>> triangle{coords[active[a]], coords[active[b]], coords[active[c]]};

                auto [is1, on1] = point_in_polygon(triangle, start);
                auto [in2, on2] = point_in_polygon(triangle, end);
                if (is1 || on1) start_face = i;
                if (in2 || on2) end_face = i;
            }

            if (dsu.find(start_face) != dsu.find(end_face)) r = mid;
            else l = mid;
        }
    }
    cout << (r == m + 1 ? -1 : id[r - 1]);
}
