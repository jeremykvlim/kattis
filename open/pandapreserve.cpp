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
double euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

template <typename T>
double squared_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return (double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y);
}

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
Point<T> midpoint(const Point<T> &a, const Point<T> &b) {
    return {a.x + (b.x - a.x) / 2, a.y + (b.y - a.y) / 2};
}

template <typename T>
Point<T> circumcenter(const array<Point<T>, 3> &triangle) {
    Point<T> a = triangle[0], b = triangle[1], c = triangle[2], ab = a - b, bc = b - c, ca = c - a;

    T d = 2 * cross(a, b, c);
    return {(squared_dist(a) * bc.y + squared_dist(b) * ca.y + squared_dist(c) * ab.y) / d, (squared_dist(a) * (-bc.x) + squared_dist(b) * (-ca.x) + squared_dist(c) * (-ab.x)) / d};
}

template <typename T>
pair<bool, bool> point_in_circumcircle(const array<Point<T>, 3> &triangle, const Point<T> &p) {
    Point<T> a = triangle[0], b = triangle[1], c = triangle[2];
    T pa = dot(a - p, a - p), pb = dot(b - p, b - p), pc = dot(c - p, c - p), det = cross(a, b, p) * pc + cross(b, c, p) * pa + cross(c, a, p) * pb;
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
Line<T> perpendicular_bisector(const Point<T> &a, const Point<T> &b) {
    Point<T> mp = midpoint(a, b), dir{a.y - b.y, b.x - a.x};
    return {mp, dir + mp};
}

template <typename T>
bool parallel(const Line<T> &l1, const Line<T> &l2) {
    return !sgn(cross(l1.b - l1.a, l2.b - l2.a));
}

template <typename T>
bool collinear(const Line<T> &l1, const Line<T> &l2) {
    return parallel(l1, l2) && !sgn(cross(l1.b - l1.a, l2.a - l1.a));
}

template <typename T>
bool intersects(const Line<T> &l1, const Line<T> &l2) {
    return collinear(l1, l2) || !parallel(l1, l2);
}

template <typename T>
Point<T> non_collinear_intersection(const Line<T> &l1, const Line<T> &l2) {
    return l1.a + (l1.b - l1.a) * cross(l2.a - l1.a, l2.b - l2.a) / cross(l1.b - l1.a, l2.b - l2.a);
}

template <typename T>
struct DelaunayTriangulation {
    struct QuadEdge {
        int dest, onext, oprev, symm;
        bool valid;

        QuadEdge() : dest(-1), onext(-1), oprev(-1), symm(-1), valid(false) {}
    };

    int start;
    vector<QuadEdge> edges;
    vector<pair<int, int>> delaunay_edges;
    vector<Point<T>> points, voronoi_vertices;
    vector<Line<T>> voronoi_edges;
    vector<int> vertex_match, edge_match;

    DelaunayTriangulation(vector<Point<T>> p, bool add_super_triangle = false) {
        if (add_super_triangle) {
            T xl = p[0].x, xr = p[0].x, yl = p[0].y, yr = p[0].y;
            for (auto [x, y] : p) {
                xl = min(xl, x);
                xr = max(xr, x);
                yl = min(yl, y);
                yr = max(yr, y);
            }

            T delta = 20 * max(xr - xl, yr - yl), xm = xl + (xr - xl) / 2, ym = yl + (yr - yl) / 2;
            p.emplace_back(xm + delta, ym - delta);
            p.emplace_back(xm, ym + delta);
            p.emplace_back(xm - delta, ym);
        }
        points = p;
        if (points.size() <= 1) return;
        guibas_stolfi();
    }

    void guibas_stolfi() {
        int n = points.size();

        vector<int> indices(n);
        iota(indices.begin(), indices.end(), 0);
        sort(indices.begin(), indices.end(), [&](int i, int j) { return points[i] < points[j]; });

        auto temp = points;
        int r = 0;
        vector<int> compress(n, 0);
        for (int i : indices)
            if (i == indices[0] || temp[i] != temp[indices[r - 1]]) {
                indices[r] = i;
                points[r++] = temp[i];
                compress[i] = i;
            } else compress[i] = indices[r - 1];

        stack<int> recycled;
        auto edge_id = [&]() -> int {
            if (!recycled.empty()) {
                int i = recycled.top();
                recycled.pop();
                return i;
            }

            edges.emplace_back();
            return edges.size() - 1;
        };

        auto unlink = [&](int i) {
            int j = edges[i].onext, k = edges[i].oprev;
            edges[j].oprev = k;
            edges[k].onext = j;
            edges[i].valid = false;
        };

        auto splice_next = [&](int i, int j) {
            int k = edges[j].onext;
            edges[i].onext = k;
            edges[i].oprev = j;
            edges[j].onext = edges[k].oprev = i;
        };

        auto splice_prev = [&](int i, int j) {
            int k = edges[j].oprev;
            edges[i].oprev = k;
            edges[i].onext = j;
            edges[j].oprev = edges[k].onext = i;
        };

        auto add_edge = [&](int u, int v) -> pair<int, int> {
            int i = edge_id(), j = edge_id();
            edges[i].onext = edges[i].oprev = edges[j].symm = i;
            edges[j].onext = edges[j].oprev = edges[i].symm = j;
            edges[i].dest = v;
            edges[j].dest = u;
            edges[i].valid = edges[j].valid = true;
            return {i, j};
        };

        auto delete_edge = [&](int i) {
            int j = edges[i].symm;
            unlink(i);
            unlink(j);
            recycled.emplace(i);
            recycled.emplace(j);
        };

        auto connect = [&](int src, int dest, int c, int d) -> pair<int, int> {
            auto [a, b] = add_edge(src, dest);
            splice_prev(a, c);
            splice_next(b, d);
            return {a, b};
        };

        auto lnext = [&](int i) -> pair<int, int> {
            return {edges[edges[i].oprev].dest, edges[edges[i].oprev].symm};
        };

        auto rprev = [&](int i) -> pair<int, int> {
            return {edges[i].dest, edges[edges[i].symm].onext};
        };

        auto min_walk = [&](pair<int, int> &p) {
            int start = p.second;
            auto q = p;
            do {
                q = rprev(q.second);
                p = min(p, q);
            } while (q.second != start);
        };

        auto max_walk = [&](pair<int, int> &p) {
            int start = p.second;
            auto q = p;
            do {
                q = rprev(q.second);
                p = max(p, q);
            } while (q.second != start);
        };

        auto left_of = [&](int a, int b, int c) {
            return cross(points[a], points[b], points[c]) > 0;
        };

        auto right_of = [&](int a, int b, int c) {
            return cross(points[a], points[b], points[c]) < 0;
        };

        auto lower_common_tangent = [&](int i, int base_i, int j, int base_j) -> array<int, 4> {
            for (;;) {
                auto [i_src, i_d] = lnext(base_i);
                if (left_of(j, i, i_src)) {
                    tie(i, base_i) = {i_src, i_d};
                    continue;
                }
                auto [j_src, j_d] = rprev(base_j);
                if (right_of(i, j, j_src)) {
                    tie(j, base_j) = {j_src, j_d};
                    continue;
                }
                break;
            }
            return {i, base_i, j, base_j};
        };

        auto in_circle = [&](int a, int b, int c, int d) {
            return point_in_circumcircle({points[a], points[b], points[c]}, points[d]).first;
        };

        auto dnc = [&](auto &&self, int l, int r) -> pair<int, int> {
            if (r - l == 2) {
                auto [i, j] = add_edge(l, l + 1);
                return {l, i};
            }

            if (r - l == 3) {
                auto [a, b] = add_edge(l, l + 1);
                auto [c, d] = add_edge(l + 1, l + 2);

                if (left_of(l, l + 1, l + 2)) {
                    auto [e, f] = add_edge(l, l + 2);
                    splice_prev(a, e);
                    splice_prev(c, b);
                    splice_prev(f, d);
                    return {l, a};
                } else if (right_of(l, l + 1, l + 2)) {
                    auto [e, f] = add_edge(l, l + 2);
                    splice_next(a, e);
                    splice_next(c, b);
                    splice_next(f, d);
                    return {l + 1, b};
                } else {
                    splice_next(b, c);
                    return {l, a};
                }
            }

            int m = l + (r - l) / 2;
            auto pl = self(self, l, m), pr = self(self, m, r);
            max_walk(pl);
            min_walk(pr);
            auto [i, base_i] = pl;
            auto [j, base_j] = pr;

            auto [ldi_src, ldi, rdi_src, rdi] = lower_common_tangent(i, base_i, j, base_j);
            auto [rdo_src, rdo, ldo_src, ldo] = lower_common_tangent(j, base_j, i, base_i);
            rdi = edges[rdi].oprev;
            rdo = edges[rdo].oprev;

            auto [base_symm, base] = connect(ldi_src, rdi_src, ldi, rdi);
            if (ldi_src == ldo_src) ldo = base_symm;
            if (rdi_src == rdo_src) rdo = base;

            while (ldi_src != ldo_src || rdi_src != rdo_src) {
                int l_dest = edges[ldi].dest, r_dest = edges[rdi].dest;

                bool vl = ldi != ldo, vr = rdi != rdo;
                if (vl && edges[ldi].onext != base_symm)
                    if (in_circle(ldi_src, rdi_src, l_dest, edges[edges[ldi].onext].dest)) {
                        int temp = edges[ldi].onext;
                        delete_edge(ldi);
                        ldi = temp;
                        continue;
                    }

                if (vr && edges[rdi].oprev != base)
                    if (in_circle(r_dest, ldi_src, rdi_src, edges[edges[rdi].oprev].dest)) {
                        int temp = edges[rdi].oprev;
                        delete_edge(rdi);
                        rdi = temp;
                        continue;
                    }

                bool v = !vr;
                if (vl && vr) {
                    if (right_of(ldi_src, rdi_src, r_dest)) v = true;
                    else if (right_of(l_dest, ldi_src, rdi_src)) v = false;
                    else v = in_circle(ldi_src, rdi_src, r_dest, l_dest);
                }

                if (v) {
                    int temp = edges[edges[ldi].symm].onext;
                    connect(l_dest, rdi_src, temp, rdi);
                    ldi = temp;
                    ldi_src = l_dest;
                } else {
                    int temp = edges[edges[rdi].symm].oprev;
                    connect(ldi_src, r_dest, ldi, temp);
                    rdi = temp;
                    rdi_src = r_dest;
                }
            }
            return {ldi_src, base_symm};
        };

        start = 0;
        if (r >= 2) start = dnc(dnc, 0, r).second;
        points = temp;

        edges.erase(remove_if(edges.begin(), edges.end(), [&](auto e) { return !e.valid; }), edges.end());
        for (auto &e : edges) e.dest = indices[e.dest];
        for (int i = 0; i < edges.size(); i++)
            if (i >= edges[i].symm) delaunay_edges.emplace_back(edges[i].dest, edges[edges[i].symm].dest);

        for (int v = 0; v < n; v++)
            if (v != compress[v]) delaunay_edges.emplace_back(v, compress[v]);
    }

    void build_voronoi_diagram() {
        if (!start) return;

        int m = edges.size();
        unordered_set<pair<int, int>, Hash> seen;
        vector<int> indices(m, -1);
        auto left_from_edge = [&](int s, int i = -1) {
            int e = s;
            do {
                if (!~i) {
                    e = edges[e].symm;
                    indices[e] = -2;
                } else {
                    auto [u, v] = minmax(indices[edges[e].symm], indices[e]);
                    vertex_match[u] = vertex_match[v] = i;
                    if (!seen.count({u, v})) {
                        seen.emplace(u, v);
                        voronoi_edges.emplace_back(voronoi_vertices[u], voronoi_vertices[v]);
                        edge_match.emplace_back(i);
                    }
                }
                e = edges[e].onext;
            } while (e != s);
        };
        left_from_edge(start);
        for (int e = 0; e < m; e++)
            if (indices[e] == -1) {
                int f = edges[edges[e].symm].oprev, g = edges[edges[f].symm].oprev;
                voronoi_vertices.emplace_back(circumcenter(array<Point<T>, 3>{points[edges[e].dest], points[edges[f].dest], points[edges[g].dest]}));
                indices[e] = indices[f] = indices[g] = voronoi_vertices.size() - 1;
            }
        for (int e = 0; e < m; e++)
            if (indices[e] == -2) {
                int f = edges[e].symm;
                if (indices[f] == -2) {
                    auto [a, b] = perpendicular_bisector(points[edges[f].dest], points[edges[e].dest]);
                    auto dir = b - a;
                    voronoi_vertices.emplace_back(a + (dir / 2));
                    voronoi_vertices.emplace_back(a - (dir / 2));
                    indices[e] = voronoi_vertices.size() - 2;
                    indices[f] = voronoi_vertices.size() - 1;
                } else {
                    auto v = points[edges[e].dest] - points[edges[f].dest];
                    voronoi_vertices.emplace_back(voronoi_vertices[indices[f]] + Point<T>(-v.y, v.x));
                    indices[e] = voronoi_vertices.size() - 1;
                }
            }
        vector<int> starts(points.size(), -1);
        for (int e = 0; e < m; e++) starts[edges[e].dest] = edges[e].symm;

        vertex_match.resize(voronoi_vertices.size());
        for (int i = 0; i < points.size(); i++)
            if (starts[i] >= 0) left_from_edge(starts[i], i);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Point<double>> polygon(n);
    for (auto &[x, y] : polygon) cin >> x >> y;

    DelaunayTriangulation<double> dt(polygon, true);
    dt.build_voronoi_diagram();
    double range = 0;
    for (int i = 0; i < dt.voronoi_vertices.size(); i++) {
        auto cc = dt.voronoi_vertices[i];
        auto [in, on] = point_in_polygon(polygon, cc);
        if (in || on) range = max(range, euclidean_dist(cc, polygon[dt.vertex_match[i]]));
    }

    for (int i = 0; i < dt.voronoi_edges.size(); i++) {
        auto l1 = dt.voronoi_edges[i];
        for (int j = 0; j < n; j++) {
            auto l2 = Line(polygon[j], polygon[(j + 1) % n]);
            if (intersects(l1, l2) && !collinear(l1, l2)) {
                auto p = non_collinear_intersection(l1, l2), l = max(min(l1.a, l1.b), min(l2.a, l2.b)), r = min(max(l1.a, l1.b), max(l2.a, l2.b));
                if (l <= p && p <= r) range = max(range, euclidean_dist(p, polygon[dt.edge_match[i]]));
            }
        }
    }

    cout << fixed << setprecision(6) << range;
}
