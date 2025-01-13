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
int sgn(const T &v) {
    return approximately_equal(v, (T) 0) ? 0 : (v > 0) - (v < 0);
}

template <typename T>
pair<bool, bool> point_in_circumcircle(const array<Point<T>, 3> &triangle, const Point<T> &p) {
    Point<T> a = triangle[0], b = triangle[1], c = triangle[2];
    T pa = dot(a - p, a - p), pb = dot(b - p, b - p), pc = dot(c - p, c - p), det = cross(a, b, p) * pc + cross(b, c, p) * pa + cross(c, a, p) * pb;
    if (sgn(det) > 0) return {true, false};
    if (!sgn(det)) return {false, true};
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
    int n;
    bool super_triangle;
    vector<Point<T>> points, voronoi_vertices;
    vector<array<int, 3>> triangles;
    vector<Line<T>> voronoi_edges;
    vector<int> edge_match;

    VoronoiDiagram(vector<Point<T>> p, bool add_super_triangle = false) : n(p.size()) {
        super_triangle = add_super_triangle;
        if (super_triangle) {
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
        delaunay_triangulation(p);
        build_diagram();
    }

    void delaunay_triangulation(vector<Point<T>> p) {
        unordered_map<Point<T>, int, typename Point<T>::Hash> indices;
        for (int i = 0; i < p.size(); i++) indices[p[i]] = i;
        sort(p.begin(), p.end());

        struct QuadEdge {
            int o, rotate;
            Point<T> s;

            QuadEdge(const Point<T> p = {numeric_limits<T>::max(), numeric_limits<T>::max()}) : s(p) {}
        };
        vector<QuadEdge> edges;

        auto symm = [&](int i) {
            return edges[edges[i].rotate].rotate;
        };

        auto onext = [&](int i) {
            return edges[i].o;
        };

        auto oprev = [&](int i) {
            return edges[onext(edges[i].rotate)].rotate;
        };

        auto lnext = [&](int i) {
            return oprev(symm(i));
        };

        auto rprev = [&](int i) {
            return onext(symm(i));
        };

        auto src = [&](int i) {
            return edges[i].s;
        };

        auto dest = [&](int i) {
            return src(symm(i));
        };

        auto splice = [&](int i, int j) {
            swap(edges[edges[edges[i].o].rotate].o, edges[edges[edges[j].o].rotate].o);
            swap(edges[i].o, edges[j].o);
        };

        auto add_edge = [&](const Point<T> &a, const Point<T> &b) {
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
        };

        auto delete_edge = [&](int i) {
            splice(i, oprev(i));
            splice(symm(i), oprev(symm(i)));
        };

        auto connect = [&](int i, int j) {
            Point<T> a = dest(i), b = src(j);

            int k = add_edge(a, b);
            splice(k, lnext(i));
            splice(symm(k), j);
            return k;
        };

        auto left_of = [&](int i, const Point<T> &p) {
            return cross(dest(i), src(i), p) < 0;
        };

        auto right_of = [&](int i, const Point<T> &p) {
            return cross(dest(i), src(i), p) > 0;
        };

        auto guibas_stolfi = [&](auto &&self, const vector<Point<T>> &s) -> pair<int, int> {
            int size = s.size();
            if (size <= 3) {
                int i = add_edge(s[0], s[1]), j = add_edge(s[1], s.back());
                if (size == 2) return {i, symm(i)};

                splice(symm(i), j);

                T cross_product = cross(s[1], s[2], s[0]);
                int k = !sgn(cross_product) ? -1 : connect(j, i);
                if (cross_product < 0) return {symm(k), k};
                return {i, symm(j)};
            }

            auto [ldo, ldi] = self(self, {s.begin(), s.begin() + size / 2});
            auto [rdo, rdi] = self(self, {s.begin() + size / 2, s.end()});

            for (;;) {
                if (left_of(ldi, src(rdo))) ldi = lnext(ldi);
                else if (right_of(rdo, src(ldi))) rdo = rprev(rdo);
                else break;
            }

            int base = connect(symm(rdo), ldi);
            if (src(ldo) == src(ldi)) ldo = symm(base);
            if (src(rdo) == src(rdi)) rdi = base;

            for (;;) {
                auto valid = [&](int i) {
                    return right_of(base, dest(i));
                };

                int lc = onext(symm(base)), rc = oprev(base);
                bool vl = valid(lc), vr = valid(rc);
                if (!vl && !vr) break;

                if (vl)
                    while (point_in_circumcircle({dest(base), src(base), dest(lc)}, dest(onext(lc))).first) {
                        int temp = onext(lc);
                        delete_edge(lc);
                        lc = temp;
                    }

                if (vr)
                    while (point_in_circumcircle({dest(base), src(base), dest(rc)}, dest(oprev(rc))).first) {
                        int temp = oprev(rc);
                        delete_edge(rc);
                        rc = temp;
                    }

                if (!vl || (vr && point_in_circumcircle({src(rc), dest(lc), src(lc)}, dest(rc)).first)) base = connect(rc, symm(base));
                else base = connect(symm(base), symm(lc));
            }
            return {ldo, rdi};
        };

        int l = guibas_stolfi(guibas_stolfi, p).first;
        queue<int> q;
        q.emplace(l);

        while (left_of(l, dest(onext(l)))) l = onext(l);

        vector<bool> visited(edges.size(), false);
        vector<Point<T>> all;
        auto left_from_edge = [&](int start, bool add) {
            int e = start;
            do {
                if (add) all.emplace_back(src(e));
                visited[e] = true;
                q.emplace(symm(e));
                e = lnext(e);
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
        vector<bool> remove;
        if (super_triangle) remove.resize(triangles.size(), false);
        voronoi_vertices.resize(triangles.size());
        unordered_map<pair<int, int>, int, Hash> seen;
        for (int i = 0; i < triangles.size(); i++) {
            auto [a, b, c] = triangles[i];
            if (super_triangle) remove[i] = !(a < n && b < n && c < n);
            voronoi_vertices[i] = circumcenter(array<Point<T>, 3>{points[a], points[b], points[c]});

            vector<pair<int, int>> triangle_edges{{a, b}, {b, c}, {c, a}};
            for (auto [u, v] : triangle_edges)
                if (u < n && v < n) {
                    if (u > v) swap(u, v);
                    if (seen.count({u, v})) {
                        int j = seen[{u, v}];
                        voronoi_edges.emplace_back(voronoi_vertices[i], voronoi_vertices[j]);
                        edge_match.emplace_back(u);
                    } else seen[{u, v}] = i;
                }
        }

        if (!super_triangle) return;
        voronoi_vertices.erase(remove_if(voronoi_vertices.begin(), voronoi_vertices.end(), [i = 0, &remove](const auto &) mutable {return remove[i++];}));
        triangles.erase(remove_if(triangles.begin(), triangles.end(), [i = 0, &remove](const auto &) mutable {return remove[i++];}));
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Point<double>> polygon(n);
    for (auto &[x, y] : polygon) cin >> x >> y;

    VoronoiDiagram<double> vd(polygon, true);
    double range = 0;
    for (int i = 0; i < vd.voronoi_vertices.size(); i++) {
        auto cc = vd.voronoi_vertices[i];
        auto [in, on] = point_in_polygon(polygon, cc);
        if (in || on) range = max(range, dist(cc, polygon[vd.triangles[i][0]]));
    }

    for (int i = 0; i < vd.voronoi_edges.size(); i++) {
        auto l1 = vd.voronoi_edges[i];
        for (int j = 0; j < n; j++) {
            auto l2 = Line(polygon[j], polygon[(j + 1) % n]);
            if (intersects(l1, l2) && !collinear(l1, l2)) {
                auto p = non_collinear_intersection(l1, l2), l = max(min(l1.a, l1.b), min(l2.a, l2.b)), r = min(max(l1.a, l1.b), max(l2.a, l2.b));
                if (l <= p && p <= r) range = max(range, dist(p, polygon[vd.edge_match[i]]));
            }
        }
    }

    cout << fixed << setprecision(6) << range;
}
