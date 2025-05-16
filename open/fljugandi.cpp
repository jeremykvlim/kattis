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
        return sgn(x - p.x) ? x < p.x : y < p.y;
    }

    bool operator>(const Point &p) const {
        return sgn(x, p.x) ? x > p.x : y > p.y;
    }

    bool operator==(const Point &p) const {
        if constexpr (is_floating_point_v<T>) return !sgn(x - p.x) && !sgn(y - p.y);
        return x == p.x && y == p.y;
    }

    bool operator!=(const Point &p) const {
        if constexpr (is_floating_point_v<T>) return sgn(x - p.x) || sgn(y - p.y);
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
double squared_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return (double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y);
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

template <typename T>
Point<T> convert_to_2D(const Point3D<T> &p, int removed_dimension = 2) {
    if (!removed_dimension) return {p.y, p.z};
    if (removed_dimension == 1) return {p.x, p.z};
    return {p.x, p.y};
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
    struct Face {
        int ca, ab, bc;

        Face() : ca(-1), ab(-1), bc(-1) {}
        Face(int ca, int ab, int bc) : ca(ca), ab(ab), bc(bc) {}
    };

    static inline mt19937 rng{random_device{}()};
    vector<Face> faces;
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
            faces.emplace_back(ca, ab, bc);
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

    vector<int> j(m);
    vector<long long> R(m);
    for (int i = 0; i < m; i++) {
        cin >> j[i] >> R[i];
        j[i]--;
    }

    vector<Point3D<__int128>> points;
    vector<long long> radius(n + 1);
    int l = 0, r = m + 1, mid;
    while (l + 1 < r) {
        mid = l + (r - l) / 2;

        points.clear();
        fill(radius.begin(), radius.end(), 0);
        for (int i = 0; i < mid; i++) radius[j[i]] = max(radius[j[i]], R[i]);

        bool blocked = false;
        unordered_map<Point3D<__int128>, int, Hash> indices;
        for (int i = 0; i < n; i++) {
            if (!radius[i]) continue;
            points.emplace_back(coords[i].x, coords[i].y, squared_dist(coords[i]) - (radius[i] * radius[i] - h * h));
            indices[points.back()] = i;

            if (radius[i] > h) {
                if (squared_dist(coords[i], start) < (radius[i] - h) * (radius[i] - h) || squared_dist(coords[i], end) < (radius[i] - h) * (radius[i] - h)) {
                    blocked = true;
                    break;
                }
            }
        }

        if (blocked) r = mid;
        else if (points.size() < 4) l = mid;
        else {
            points.emplace_back(0, 0, 1e18);
            ConvexHull3D convex_hull(points);
            int s = convex_hull.faces.size();
            DisjointSets dsu(s + 1);
            vector<bool> upward(s, false);
            for (int f : convex_hull.valid_faces) {
                auto [a, b, c] = convex_hull.face_vertices(f);
                upward[f] = cross(points[a], points[b], points[c]).z >= 0;
            }

            for (int f : convex_hull.valid_faces) {
                auto [ca, ab, bc] = convex_hull.faces[f];

                for (int ji : {ca, ab, bc}) {
                    int ij = ji ^ 1, i = convex_hull.edge_dest[ji], j = convex_hull.edge_dest[ij], g = convex_hull.edge_face[ij];

                    auto ri = radius[indices[points[i]]], rj = radius[indices[points[j]]];
                    if (!(ri > h && rj > h) ||
                        euclidean_dist(convert_to_2D(points[i]), convert_to_2D(points[j])) >= sqrt(ri * ri - h * h) + sqrt(rj * rj - h * h))
                        dsu.unite(f, (!~g || upward[g]) ? s : g);
                }
            }

            int start_face = s, end_face = s;
            for (int f : convex_hull.valid_faces)
                if (!upward[f]) {
                    auto [a, b, c] = convex_hull.face_vertices(f);
                    auto a_2D = convert_to_2D(points[a]), b_2D = convert_to_2D(points[b]), c_2D = convert_to_2D(points[c]);
                    vector<Point<long long>> triangle{a_2D, b_2D, c_2D};

                    auto [is1, on1] = point_in_polygon(triangle, start);
                    auto [in2, on2] = point_in_polygon(triangle, end);
                    if (is1 || on1) start_face = f;
                    if (in2 || on2) end_face = f;
                }

            if (dsu.find(start_face) != dsu.find(end_face)) r = mid;
            else l = mid;
        }
    }
    cout << (r == m + 1 ? -1 : r);
}
