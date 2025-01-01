#include <bits/stdc++.h>
using namespace std;

template <typename T>
bool approximately_equal(const T &v1, const T &v2) {
    return fabs(v1 - v2) <= 1e-8;
}

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    template <typename U>
    Point(const Point<U> &p) : x((T) p.x), y((T) p.y) {}

    Point operator-() const {
        return {-x, -y};
    }

    bool operator<(const Point &p) const {
        return !approximately_equal(x, p.x) ? x < p.x : y < p.y;
    }

    bool operator>(const Point &p) const {
        return !approximately_equal(x, p.x) ? x > p.x : y > p.y;
    }

    bool operator==(const Point &p) const {
        if constexpr (is_floating_point_v<T>) return approximately_equal(x, p.x) && approximately_equal(y, p.y);
        return x == p.x && y == p.y;
    }

    bool operator!=(const Point &p) const {
        if constexpr (is_floating_point_v<T>) return !approximately_equal(x, p.x) || !approximately_equal(y, p.y);
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

    struct Hash {
        size_t operator()(Point<T> p) const {
            auto h = 0ULL;
            h ^= hash<T>()(p.x) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= hash<T>()(p.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
            return h;
        }
    };
};

template <typename T>
double dist(const Point<T> &a, const Point<T> &b) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

template <typename T>
T squared_dist(const Point<T> &p) {
    return p.x * p.x + p.y * p.y;
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
Point<T> circumcenter(const array<Point<T>, 3> &triangle) {
    Point<T> a = triangle[0], b = triangle[1], c = triangle[2], ab = a - b, bc = b - c, ca = c - a;

    T d = 2 * cross(a, b, c);
    return {(squared_dist(a) * bc.y + squared_dist(b) * ca.y + squared_dist(c) * ab.y) / d, (squared_dist(a) * (-bc.x) + squared_dist(b) * (-ca.x) + squared_dist(c) * (-ab.x)) / d};
}

template <typename T>
pair<bool, bool> point_in_circumcircle(const array<Point<T>, 3> &triangle, const Point<T> &p) {
    Point<T> a = triangle[0], b = triangle[1], c = triangle[2];
    T pa = dot(a - p, a - p), pb = dot(b - p, b - p), pc = dot(c - p, c - p);

    T det = cross(a, b, p) * pc + cross(b, c, p) * pa + cross(c, a, p) * pb;
    if (det > 0) return {true, false};
    if (approximately_equal(det, (T) 0)) return {false, true};
    return {false, false};
}

template <typename T>
int sgn(const T &v) {
    return approximately_equal(v, (T) 0) ? 0 : (v > 0) - (v < 0);
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
    Line(T &a, T &b, T &c) {
        if (fabs(a) < 1e-8 && fabs(b) > 1e-8) {
            a = {0, c / b};
            b = {1, c / b};
        } else if (fabs(b) < 1e-8) {
            a = {c / a, 0};
            b = {c / a, 1};
        } else if (fabs(c) < 1e-8) {
            a = {0, c / b};
            b = {1, (c - a) / b};
        } else {
            a = {0, c / b};
            b = {c / a, 0};
        }
    }
};

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
    return l2.a + (l2.b - l2.a) * cross(l1.b - l1.a, l1.b - l2.a) / cross(l1.b - l1.a, l2.b - l2.a);
}

struct Hash {
    size_t operator()(pair<int, int> p) const {
        auto h = hash<int>()(p.first);
        h ^= hash<int>()(p.second) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

template <typename T>
struct VoronoiDiagram {
    struct QuadEdge {
        int o, rotate;
        Point<T> s;

        QuadEdge(const Point<T> p = {numeric_limits<T>::max(), numeric_limits<T>::max()}) : s(p) {}
    };
    vector<QuadEdge> edges;

    int add_edge(const Point<T> &a, const Point<T> &b) {
        int i = edges.size();
        edges.emplace_back(a);
        edges.emplace_back();
        edges.emplace_back(b);
        edges.emplace_back();

        edges[i].o = edges[i + 3].rotate = i;
        edges[i].rotate = edges[i + 3].o = i + 1;
        edges[i + 1].o = edges[i + 2].rotate = i + 3;
        edges[i + 1].rotate = edges[i + 2].o = i + 2;
        return i;
    }

    void splice(int i, int j) {
        swap(edges[edges[edges[i].o].rotate].o, edges[edges[edges[j].o].rotate].o);
        swap(edges[i].o, edges[j].o);
    }

    int connect(int i, int j) {
        Point<T> a = dest(edges[i]), b = src(edges[j]);

        int k = add_edge(a, b);
        splice(k, lnext(edges[i]));
        splice(symm(edges[k]), j);
        return k;
    }

    int symm(QuadEdge e) const {
        return edges[e.rotate].rotate;
    }

    int onext(QuadEdge e) const {
        return e.o;
    }

    int oprev(QuadEdge e) const {
        return edges[edges[e.rotate].o].rotate;
    }

    int lnext(QuadEdge e) const {
        return oprev(edges[symm(e)]);
    }

    int rprev(QuadEdge e) const {
        return edges[symm(e)].o;
    }

    Point<T> src(QuadEdge e) const {
        return e.s;
    }

    Point<T> dest(QuadEdge e) const {
        return edges[symm(e)].s;
    }

    int n;
    vector<bool> visited;
    vector<Point<T>> points, voronoi_vertices;
    vector<array<int, 3>> triangles;
    vector<Line<T>> voronoi_edges;
    vector<int> edge_match;

    VoronoiDiagram(vector<Point<T>> p, bool add_super_triangle = false) : n(p.size()) {
        if (add_super_triangle) {
            T xl = p[0].x, xr = p[0].x, yl = p[0].y, yr = p[0].y;
            for (auto [x, y] : p) {
                xl = min(xl, x);
                xr = max(xr, x);
                yl = min(yl, y);
                yr = max(yr, y);
            }

            T delta = 20 * max(xr - xl, yr - yl), xm = xl + (xr - xl) / 2, ym = yl + (yr - yl) / 2;
            p.emplace_back(xm + delta, ym + delta);
            p.emplace_back(xm, ym - delta);
            p.emplace_back(xm - delta, ym);
        }
        points = p;
        delaunay_triangulation(p);
        build_diagram();
    }

    void delaunay_triangulation(vector<Point<T>> p) {
        unordered_map<Point<T>, int, typename Point<T>::Hash> indices;
        for (int i = 0; i < p.size(); i++) indices[p[i]] = i;

        sort(p.begin(), p.end());
        auto guibas_stolfi = [&](auto &&self, const vector<Point<T>> &v) -> pair<int, int> {
            int size = v.size();
            if (size <= 3) {
                int i = add_edge(v[0], v[1]), j = add_edge(v[1], v.back());
                if (size == 2) return {i, symm(edges[i])};

                splice(symm(edges[i]), j);

                T cross_product = cross(v[1], v[2], v[0]);
                int k = !approximately_equal(cross_product, (T) 0) ? connect(j, i) : -1;
                if (cross_product < 0) return {symm(edges[k]), k};
                return {i, symm(edges[j])};
            }

            auto [ll, lr] = self(self, {v.begin(), v.begin() + size / 2});
            auto [rl, rr] = self(self, {v.begin() + size / 2, v.end()});

            auto lowest_common_tangent = [&]() {
                for (;;) {
                    if (cross(dest(edges[lr]), src(edges[lr]), src(edges[rl])) < 0) {
                        lr = lnext(edges[lr]);
                        continue;
                    }

                    if (cross(dest(edges[rl]), src(edges[rl]), src(edges[lr])) > 0) {
                        rl = rprev(edges[rl]);
                        continue;
                    }
                    break;
                }
            };
            lowest_common_tangent();

            int base = connect(symm(edges[rl]), lr);
            if (src(edges[ll]) == src(edges[lr])) ll = symm(edges[base]);
            if (src(edges[rl]) == src(edges[rr])) rr = base;

            for (;;) {
                auto &be = edges[base];
                auto valid = [&](int i) {
                    return cross(dest(be), src(be), dest(edges[i])) > 0;
                };

                int lc = onext(edges[symm(be)]), rc = oprev(be);
                bool vl = valid(lc), vr = valid(rc);
                if (!vl && !vr) break;

                auto remove = [&](int c, auto e) {
                    splice(c, oprev(e));
                    splice(symm(e), oprev(edges[symm(e)]));
                };

                if (vl)
                    while (point_in_circumcircle({dest(be), src(be), dest(edges[lc])}, dest(edges[onext(edges[lc])])).first) {
                        auto &e = edges[lc];
                        int temp = onext(e);
                        remove(lc, e);
                        lc = temp;
                    }

                if (vr)
                    while (point_in_circumcircle({dest(be), src(be), dest(edges[rc])}, dest(edges[oprev(edges[rc])])).first) {
                        auto &e = edges[rc];
                        int temp = oprev(e);
                        remove(rc, e);
                        rc = temp;
                    }

                auto le = edges[lc], re = edges[rc];
                if (!vl || (vr && point_in_circumcircle({src(re), dest(le), src(le)}, dest(re)).first)) base = connect(rc, symm(be));
                else base = connect(symm(be), symm(le));
            }
            return {ll, rr};
        };

        int l = guibas_stolfi(guibas_stolfi, p).first;
        visited.resize(edges.size(), false);
        queue<int> q;
        q.emplace(l);

        while (cross(dest(edges[l]), src(edges[l]), dest(edges[onext(edges[l])])) < 0) l = onext(edges[l]);

        vector<Point<T>> all;
        auto left_from_edge = [&](int start, bool add) {
            int e = start;
            do {
                if (add) all.emplace_back(src(edges[e]));
                visited[e] = true;
                q.emplace(symm(edges[e]));
                e = lnext(edges[e]);
            } while (e != start);
        };
        left_from_edge(l, false);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            if (!visited[v]) left_from_edge(v, true);
        }

        for (int i = 0; i + 2 < all.size(); i += 3) triangles.push_back({indices[all[i]], indices[all[i + 1]], indices[all[i + 2]]});
    }

    void build_diagram() {
        voronoi_vertices.resize(triangles.size());
        vector<bool> remove(triangles.size(), false);
        for (int i = 0; i < triangles.size(); i++) {
            auto [a, b, c] = triangles[i];
            remove[i] = !(a < n && b < n && c < n);
            voronoi_vertices[i] = circumcenter(array<Point<T>, 3>{points[a], points[b], points[c]});
        }

        unordered_map<pair<int, int>, vector<int>, Hash> edge_to_triangles;
        for (int i = 0; i < triangles.size(); i++)
            for (int j = 0; j < 3; j++) {
                int a = triangles[i][j], b = triangles[i][(j + 1) % 3];
                if (a < n && b < n) {
                    if (a > b) swap(a, b);
                    edge_to_triangles[{a, b}].emplace_back(i);
                }
            }

        for (auto [edge, indices] : edge_to_triangles)
            if (indices.size() == 2) {
                auto [a, b] = edge;
                voronoi_edges.emplace_back(voronoi_vertices[indices[0]], voronoi_vertices[indices[1]]);
                edge_match.emplace_back(a);
            }

        voronoi_vertices.erase(remove_if(voronoi_vertices.begin(), voronoi_vertices.end(), [i = 0, &remove](const auto &) mutable {return remove[i++];}));
        triangles.erase(remove_if(triangles.begin(), triangles.end(), [i = 0, &remove](const auto &) mutable {return remove[i++];}));
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Point<double>> coords(n);
    for (auto &[x, y] : coords) cin >> x >> y;

    VoronoiDiagram<double> vd(coords, true);
    double range = 0;
    for (int i = 0; i < vd.voronoi_vertices.size(); i++) {
        auto cc = vd.voronoi_vertices[i];
        auto [in, on] = point_in_polygon(coords, cc);
        if (in || on) range = max(range, dist(cc, coords[vd.triangles[i][0]]));
    }

    for (int i = 0; i < vd.voronoi_edges.size(); i++) {
        auto l1 = vd.voronoi_edges[i];
        for (int j = 0; j < n; j++) {
            auto l2 = Line(coords[j], coords[(j + 1) % n]);
            if (intersects(l1, l2) && !collinear(l1, l2)) {
                auto p = non_collinear_intersection(l1, l2), l = max(min(l1.a, l1.b), min(l2.a, l2.b)), r = min(max(l1.a, l1.b), max(l2.a, l2.b));
                if (l <= p && p <= r) range = max(range, dist(p, coords[vd.edge_match[i]]));
            }
        }
    }

    cout << fixed << setprecision(6) << range;
}
