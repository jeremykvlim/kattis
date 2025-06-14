#include <bits/stdc++.h>
using namespace std;

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
        if constexpr (is_floating_point_v<T>) return !approximately_equal(x, p.x) ? sgn(x - p.x) == -1 : sgn(y - p.y) == -1;
        return x != p.x ? x < p.x : y < p.y;
    }

    bool operator>(const Point &p) const {
        if constexpr (is_floating_point_v<T>) return !approximately_equal(x, p.x) ? sgn(x - p.x) == 1 : sgn(y - p.y) == 1;
        return x != p.x ? x > p.x : y > p.y;
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
};

template <typename T>
T cross(const Point<T> &a, const Point<T> &b, const Point<T> &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

template <typename T>
double euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

template <typename T>
double squared_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return (double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y);
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
        return sgn(x - p.x) ? x < p.x : (sgn(y - p.y) ? y < p.y : z < p.z);
    }

    bool operator>(const Point3D &p) const {
        return sgn(x - p.x) ? x > p.x : (sgn(y - p.y) ? y > p.y : z > p.z);
    }

    bool operator==(const Point3D &p) const {
        if constexpr (is_floating_point_v<T>) return !sgn(x - p.x) && !sgn(y - p.y) && !sgn(z - p.z);
        return x == p.x && y == p.y && z == p.z;
    }

    bool operator!=(const Point3D &p) const {
        if constexpr (is_floating_point_v<T>) return sgn(x - p.x) || sgn(y - p.y) || sgn(z - p.z);
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
T dot(const Point3D<T> &a, const Point3D<T> &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <typename T>
double euclidean_dist(const Point3D<T> &a, const Point3D<T> &b = {0, 0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y) + (double) (a.z - b.z) * (a.z - b.z));
}

template <typename T>
Point3D<T> cross(const Point3D<T> &a, const Point3D<T> &b) {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

template <typename T>
Point3D<T> cross(const Point3D<T> &a, const Point3D<T> &b, const Point3D<T> &c) {
    return cross(b - a, c - a);
}

template <typename T>
bool collinear(const Point3D<T> &a, const Point3D<T> &b, const Point3D<T> &c) {
    if (a == b || b == c || a == c) return true;
    auto p = cross(a, b, c);
    return !sgn(p.x) && !sgn(p.y) && !sgn(p.z);
}

template <typename T>
T signed_volume_of_parallelepiped(const Point3D<T> &a, const Point3D<T> &b, const Point3D<T> &c, const Point3D<T> &d) {
    return dot(cross(b - a, c - a), d - a);
}

struct ForwardStar {
    vector<int> head, next;

    ForwardStar() {}
    ForwardStar(int n, int m) : head(n, -1), next(m) {}

    void extend() {
        head.emplace_back(-1);
    }

    void add_edge(int u, int edge_id) {
        if (next.size() < edge_id + 1) next.resize(edge_id + 1);
        next[edge_id] = head[u];
        head[u] = edge_id;
    }
};

template <typename T>
struct ConvexHull3D {
    static inline mt19937 rng{random_device{}()};
    vector<array<int, 3>> faces;
    vector<int> valid_faces, edge_dest, edge_face;

    ConvexHull3D(vector<Point3D<T>> &points) {
        sort(points.begin(), points.end());
        points.erase(unique(points.begin(), points.end()), points.end());

        int n = points.size(), i = 2;
        for (; i < n; i++)
            if (!collinear(points[0], points[1], points[i])) {
                swap(points[i], points[2]);
                break;
            }

        for (++i; i < n; i++)
            if (sgn(signed_volume_of_parallelepiped(points[0], points[1], points[2], points[i]))) {
                swap(points[i], points[3]);
                break;
            }

        if (sgn(signed_volume_of_parallelepiped(points[0], points[1], points[2], points[3])) < 0) swap(points[1], points[2]);
        quickhull(n, points);
    }

    void quickhull(int n, const vector<Point3D<T>> &points) {
        auto add_edge = [&](int dest) {
            edge_dest.emplace_back(dest);
            edge_face.emplace_back(-1);
        };

        for (int i = 0; i < 4; i++)
            for (int j = i + 1; j < 4; j++) {
                add_edge(j);
                add_edge(i);
            }

        stack<int> s;
        ForwardStar list;
        vector<int> state;
        vector<bool> invalid;
        auto add_face = [&](int ca, int ab, int bc) {
            int i = faces.size();
            faces.push_back({ca, ab, bc});
            valid_faces.emplace_back(i);
            edge_face[ca] = edge_face[ab] = edge_face[bc] = i;
            s.emplace(i);
            list.extend();
            state.emplace_back(0);
            invalid.emplace_back(false);
        };
        add_face(7, 1, 2);
        add_face(5, 0, 8);
        add_face(9, 6, 10);
        add_face(11, 3, 4);

        auto face_visible_from_point = [&](int f, int d) -> bool {
            auto [a, b, c] = face_vertices(f);
            return sgn(signed_volume_of_parallelepiped(points[a], points[b], points[c], points[d])) > 0;
        };

        vector<int> conflict_points;
        for (int i = 4; i < n; i++)
            for (int f : valid_faces)
                if (face_visible_from_point(f, i)) {
                    conflict_points.emplace_back(i);
                    list.add_edge(f, conflict_points.size() - 1);
                    break;
                }

        vector<int> temp(n);
        while (!s.empty()) {
            int v = s.top();
            s.pop();

            if (state[v] == 1 || !~list.head[v]) continue;

            auto pivot = [&]() {
                auto [a, b, c] = face_vertices(v);
                T prev = numeric_limits<T>::lowest();
                int p = -1;
                for (int i = list.head[v]; ~i; i = list.next[i]) {
                    int d = conflict_points[i];
                    auto vol = signed_volume_of_parallelepiped(points[a], points[b], points[c], points[d]);
                    if (prev < vol) {
                        prev = vol;
                        p = d;
                    }
                }
                return p;
            };
            int p = pivot();

            state[v] = 1;
            vector<int> visible{v}, non_visible, horizon;
            for (int i = 0; i < visible.size(); i++) {
                int f = visible[i];
                auto [ca, ab, bc] = faces[f];
                for (int e : {ca, ab, bc}) {
                    int g = edge_face[e ^ 1];
                    if (!state[g]) {
                        if (face_visible_from_point(g, p)) {
                            state[g] = 1;
                            visible.emplace_back(g);
                        } else {
                            state[g] = 2;
                            non_visible.emplace_back(g);
                        }
                    }

                    if (state[g] == 2) {
                        temp[edge_dest[e]] = edge_dest.size();
                        add_edge(p);
                        add_edge(edge_dest[e]);
                        horizon.emplace_back(e);
                    }
                }
            }
            for (int e : horizon) add_face(temp[edge_dest[e]], temp[edge_dest[e ^ 1]] ^ 1, e);

            int count = 0;
            for (int f : visible)
                for (int i = list.head[f]; ~i; i = list.next[i])
                    if (conflict_points[i] != p) {
                        if (++count == 100) {
                            shuffle(horizon.begin(), horizon.end(), rng);
                            count = 0;
                        }

                        for (int e : horizon)
                            if (face_visible_from_point(edge_face[e], conflict_points[i])) {
                                conflict_points.emplace_back(conflict_points[i]);
                                list.add_edge(edge_face[e], conflict_points.size() - 1);
                                break;
                            }
                    }

            for (int f : visible) {
                invalid[f] = true;
                list.head[f] = -1;
            }
            for (int f : non_visible) state[f] = 0;
        }

        valid_faces.erase(remove_if(valid_faces.begin(), valid_faces.end(), [&](int i) { return invalid[i]; }), valid_faces.end());
    }

    array<int, 3> face_vertices(int f) {
        auto [ca, ab, bc] = faces[f];
        return {edge_dest[ca], edge_dest[ab], edge_dest[bc]};
    }
};

template <typename T>
struct Circle {
    Point<T> origin;
    T radius;

    Circle() {}
    Circle(const Point<T> &o, const T &r) : origin(o), radius(r) {}
};

template <typename T>
T radius_of_smallest_enclosing_circle(vector<Point<T>> &P) {
    shuffle(P.begin(), P.end(), mt19937_64(random_device{}()));
    vector<Point<T>> R;
    auto welzl = [&](auto &&self, int n) -> Circle<T> {
        if (!n || R.size() == 3) {
            Point<T> o{0, 0};
            T r = 0;
            if (R.size() == 1) o = R[0];
            else if (R.size() == 2) {
                o = midpoint(R[0], R[1]);
                r = euclidean_dist(o, R[0]);
            } else if (R.size() == 3) {
                o = circumcenter(array<Point<T>, 3>{R[0], R[1], R[2]});
                r = euclidean_dist(o, R[0]);
            }
            return {o, r};
        }

        auto p = P[n - 1];
        auto C1 = self(self, n - 1);
        if (euclidean_dist(C1.origin, p) <= C1.radius) return C1;

        R.emplace_back(p);
        auto C2 = self(self, n - 1);
        R.pop_back();
        return C2;
    };
    return welzl(welzl, P.size()).radius;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Point3D<double>> points(n);
    for (auto &[x, y, z] : points) cin >> x >> y >> z;

    auto volume = 1e20;
    vector<Point<double>> projections(n);
    ConvexHull3D convex_hull(points);
    for (int f : convex_hull.valid_faces) {
        auto [a, b, c] = convex_hull.face_vertices(f);

        auto orthonormal_basis = [&]() -> array<Point3D<double>, 3> {
            auto normal = cross(points[a], points[b], points[c]), u = points[b] - points[a];
            auto normalize = [&](auto &p) {
                p /= euclidean_dist(p);
            };
            normalize(normal);
            normalize(u);
            auto v = cross(normal, u);
            return {u, v, normal};
        };
        auto [u, v, normal] = orthonormal_basis();

        double h = 0;
        for (int i = 0; i < n; i++) {
            projections[i] = {dot(points[i], u), dot(points[i], v)};
            h = max(h, fabs(dot(points[a] - points[i], normal)));
        }

        auto r = radius_of_smallest_enclosing_circle(projections);
        volume = min(volume, M_PI * r * r * h);
    }
    cout << fixed << setprecision(6) << volume;
}
