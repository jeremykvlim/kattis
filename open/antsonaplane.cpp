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
    if constexpr (!is_floating_point_v<T>) return (v > 0) - (v < 0);
    else return approximately_equal(v, (T) 0) ? 0 : (v > 0) - (v < 0);
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

    Point operator!() const {
        return {y, x};
    }

    Point operator~() const {
        return {-y, x};
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
double angle(const Point<T> &p) {
    return atan2(p.y, p.x);
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
T doubled_signed_area_of_polygon(const vector<Point<T>> &polygon) {
    T area = 0;
    for (int i = 0; i < polygon.size(); i++) area += cross(polygon[i], polygon[(i + 1) % polygon.size()]);
    return area;
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

    const auto begin() const {
        return &a;
    }

    const auto end() const {
        return &b + 1;
    }

    bool operator==(const Line &l) const {
        return a == l.a && b == l.b;
    }

    bool operator!=(const Line &l) const {
        return a != l.a || b != l.b;
    }

    bool operator<(const Line &l) const {
        return a != l.a ? a < l.a : b < l.b;
    }

    bool operator>(const Line &l) const {
        return a != l.a ? a > l.a : b > l.b;
    }

    bool operator<=(const Line &l) const {
        return *this < l || *this == l;
    }

    bool operator>=(const Line &l) const {
        return *this > l || *this == l;
    }
};

template <typename T>
bool point_on_line(const Line<T> &l, const Point<T> &p, bool include_endpoints = true) {
    return !sgn(cross(l.b - l.a, p - l.a)) && (dot(l.a - p, l.b - p) < 0 || (include_endpoints && approximately_equal(dot(l.a - p, l.b - p), (T) 0)));
}

template <typename T>
bool parallel(const Line<T> &l1, const Line<T> &l2) {
    return !sgn(cross(l1.b - l1.a, l2.b - l2.a));
}

template <typename T>
Point<T> non_collinear_intersection(const Line<T> &l1, const Line<T> &l2) {
    return l1.a + (l1.b - l1.a) * cross(l2.a - l1.a, l2.b - l2.a) / cross(l1.b - l1.a, l2.b - l2.a);
}

struct Treap {
    static inline mt19937_64 rng{random_device{}()};

    struct TreapNode {
        array<int, 3> family;
        unsigned long long prio;
        int key;

        TreapNode(int k = 0) : family{0, 0, 0}, prio(rng()), key(k) {}
    };

    int root;
    vector<TreapNode> T;
    stack<int> recycled;
    function<bool(int, int)> cmp_less;

    Treap(const function<bool(int, int)> &cmp_less) : root(0), T(1), cmp_less(cmp_less) {}

    int node(const int &key) {
        int i;
        if (!recycled.empty()) {
            i = recycled.top();
            recycled.pop();
        } else {
            T.emplace_back();
            i = T.size() - 1;
        }
        T[i] = TreapNode(key);
        return i;
    }

    int pull(int i) {
        if (!i) return 0;
        return i;
    }

    void attach(int i, int c, int j) {
        T[i].family[c] = j;
        if (j) T[j].family[2] = i;
        pull(i);
    }

    pair<int, int> split(int i, const int &key) {
        if (!i) return {0, 0};
        auto [l, r, p] = T[i].family;
        if (cmp_less(key, T[i].key)) {
            auto [ll, lr] = split(l, key);
            attach(i, 0, lr);
            if (ll) T[ll].family[2] = 0;
            T[i].family[2] = 0;
            return {ll, i};
        } else {
            auto [rl, rr] = split(r, key);
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

    int insert(const int &key) {
        int i = node(key);
        auto [l, r] = split(root, key);
        root = meld(meld(l, i), r);
        T[root].family[2] = 0;
        return i;
    }

    int erase_node(int i) {
        if (!i) return 0;
        auto [l, r, p] = T[i].family;
        int m = meld(l, r);
        T[m].family[2] = p;
        recycled.emplace(i);
        (!p ? root : T[p].family[T[p].family[0] != i]) = m;
        for (; p; p = T[p].family[2]) pull(p);
        T[root].family[2] = 0;
        return root;
    }

    int lower_bound(const int &key) const {
        int k = 0;
        for (int i = root; i;)
            if (!cmp_less(T[i].key, key)) {
                k = i;
                i = T[i].family[0];
            } else i = T[i].family[1];
        return k;
    }

    int predecessor(int i) const {
        if (!i) return 0;
        if (T[i].family[0]) {
            for (i = T[i].family[0]; T[i].family[1]; i = T[i].family[1]);
            return i;
        }
        int p = T[i].family[2];
        for (; p && T[p].family[0] == i; i = p, p = T[p].family[2]);
        return p;
    }

    int successor(int i) const {
        if (!i) return 0;
        if (T[i].family[1]) {
            for (i = T[i].family[1]; T[i].family[0]; i = T[i].family[0]);
            return i;
        }
        int p = T[i].family[2];
        for (; p && T[p].family[1] == i; i = p, p = T[p].family[2]);
        return p;
    }

    auto & operator[](int i) {
        return T[i];
    }
};

template <typename T>
vector<pair<Point<T>, vector<int>>> bentley_ottmann(const vector<Line<T>> &lines) {
    auto deduped = lines;
    sort(deduped.begin(), deduped.end());
    deduped.erase(unique(deduped.begin(), deduped.end()), deduped.end());

    int n = deduped.size();
    vector<vector<int>> original_indices(n);
    for (int i = 0; i < lines.size(); i++) original_indices[lower_bound(deduped.begin(), deduped.end(), lines[i]) - deduped.begin()].emplace_back(i);

    vector<Point<T>> dirs(n);
    for (int i = 0; i < n; i++) dirs[i] = deduped[i].b - deduped[i].a;

    vector<pair<Point<T>, int>> events;
    vector<bool> vertical;
    map<Point<T>, array<vector<int>, 4>> sweep;
    for (int i = 0; i < n; i++) {
        const auto &[a, b] = deduped[i];

        if (a.x == b.x) {
            events.emplace_back(a, i);
            vertical.emplace_back(true);
            sweep[a][1].emplace_back(events.size() - 1);
        } else {
            events.emplace_back(a, i);
            vertical.emplace_back(false);
            sweep[a][0].emplace_back(events.size() - 1);
            events.emplace_back(b, i);
            vertical.emplace_back(false);
            sweep[b][3].emplace_back(events.size() - 1);
        }
    }

    T x = 0;
    auto y_at_x = [&](const auto &l) {
        auto &[a, b] = l;
        if (x <= a.x) return a.y;
        if (x >= b.x) return b.y;
        return a.y + (b.y - a.y) * (x - a.x) / (b.x - a.x);
    };

    bool left = true;
    Treap treap([&](int e1, int e2) -> bool {
        if (e1 == e2) return false;
        const auto &[a, i] = events[e1];
        const auto &[b, j] = events[e2];

        auto &l1 = deduped[i], &l2 = deduped[j];
        int s = sgn((vertical[e1] ? a.y : y_at_x(l1)) - (vertical[e2] ? b.y : y_at_x(l2)));
        if (s) return s < 0;

        T cp = cross(dirs[i], dirs[j]);
        if (cp > 0) return !left;
        if (cp < 0) return left;
        return i < j;
    });
    vector<int> event_node(events.size(), 0);
    vector<bool> active(events.size(), false);
    unordered_map<Point<T>, vector<int>, Hash> intersections_map;
    auto check_intersection = [&](int e1, int e2) {
        if (e1 == e2) return;
        const auto &[a, i] = events[e1];
        const auto &[b, j] = events[e2];

        auto &l1 = deduped[i], &l2 = deduped[j];
        if (parallel(l1, l2)) return;

        auto p = non_collinear_intersection(l1, l2);
        if (!point_on_line(l1, p, true) || !point_on_line(l2, p, true)) return;

        auto [it, inserted] = intersections_map.try_emplace(p);
        if (inserted && x <= p.x) sweep[p][2].emplace_back();
        it->second.emplace_back(e1);
        it->second.emplace_back(e2);
    };

    while (!sweep.empty()) {
        x = sweep.begin()->first.x;

        vector<pair<Point<T>, array<vector<int>, 4>>> batch;
        while (!sweep.empty() && sweep.begin()->first.x == x) {
            batch.emplace_back(*sweep.begin());
            sweep.erase(sweep.begin());
        }

        for (auto &[p, buckets] : batch)
            for (int e : buckets[0]) {
                left = false;
                if (!active[e]) {
                    event_node[e] = treap.insert(e);
                    active[e] = true;
                }

                int node = event_node[e], prev = treap.predecessor(node), next = treap.successor(node);
                if (prev) check_intersection(e, treap[prev].key);
                if (next) check_intersection(e, treap[next].key);
            }

        for (auto &[p, buckets] : batch)
            for (int e1 : buckets[1]) {
                const auto &[a, i] = events[e1];
                left = true;
                T y_max = deduped[i].b.y;

                for (int node = treap.lower_bound(e1); node; node = treap.successor(node)) {
                    int e2 = treap[node].key;
                    const auto &[b, j] = events[e2];
                    if ((vertical[e2] ? b.y : y_at_x(deduped[j])) > y_max) break;
                    check_intersection(e1, e2);
                }
            }

        for (auto &[p, buckets] : batch)
            if (!buckets[2].empty()) {
                left = true;
                auto it = intersections_map.find(p);
                if (it == intersections_map.end()) continue;

                stack<int> s;
                for (int e2 : it->second)
                    if (active[e2]) {
                        treap.erase_node(event_node[e2]);
                        active[e2] = false;
                        s.emplace(e2);
                    }

                left = false;
                while (!s.empty()) {
                    int e2 = s.top();
                    s.pop();

                    int node = event_node[e2] = treap.insert(e2), prev = treap.predecessor(node), next = treap.successor(node);
                    active[e2] = true;
                    if (prev) check_intersection(e2, treap[prev].key);
                    if (next) check_intersection(e2, treap[next].key);
                }
            }

        for (;;) {
            vector<pair<Point<T>, array<vector<int>, 4>>> extra;
            while (!sweep.empty() && sweep.begin()->first.x == x) {
                extra.emplace_back(*sweep.begin());
                sweep.erase(sweep.begin());
            }
            if (extra.empty()) break;

            for (auto &[p, buckets] : extra)
                if (!buckets[2].empty()) {
                    left = true;
                    auto it = intersections_map.find(p);
                    if (it == intersections_map.end()) continue;

                    stack<int> s;
                    for (int e2 : it->second)
                        if (active[e2]) {
                            treap.erase_node(event_node[e2]);
                            active[e2] = false;
                            s.emplace(e2);
                        }

                    left = false;
                    while (!s.empty()) {
                        int e2 = s.top();
                        s.pop();

                        int node = event_node[e2] = treap.insert(e2), prev = treap.predecessor(node), next = treap.successor(node);
                        active[e2] = true;
                        if (prev) check_intersection(e2, treap[prev].key);
                        if (next) check_intersection(e2, treap[next].key);
                    }
                }
        }

        for (auto &[p, buckets] : batch)
            for (int e : buckets[3]) {
                left = true;
                if (!active[e - 1]) continue;

                int node = event_node[e - 1], prev = treap.predecessor(node), next = treap.successor(node);
                treap.erase_node(node);
                active[e - 1] = false;
                if (prev && next) check_intersection(treap[prev].key, treap[next].key);
            }
    }

    vector<pair<Point<T>, vector<int>>> intersections;
    for (auto &[p, event_ids] : intersections_map) {
        vector<int> deduped_indices;
        for (int e : event_ids) deduped_indices.emplace_back(events[e].second);
        sort(deduped_indices.begin(), deduped_indices.end());
        deduped_indices.erase(unique(deduped_indices.begin(), deduped_indices.end()), deduped_indices.end());

        vector<int> indices;
        for (int i : deduped_indices) indices.insert(indices.end(), original_indices[i].begin(), original_indices[i].end());
        intersections.emplace_back(p, indices);
    }
    return intersections;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Line<long double>> lines(n);
    map<Point<long double>, vector<int>> all;
    for (int i = 0; i < n; i++) {
        cin >> lines[i].a.x >> lines[i].a.y >> lines[i].b.x >> lines[i].b.y;

        if (lines[i].a > lines[i].b) swap(lines[i].a, lines[i].b);
        all[lines[i].a].emplace_back(i);
        all[lines[i].b].emplace_back(i);
    }
    for (auto &[p, indices] : bentley_ottmann(lines)) all[p].insert(all[p].end(), indices.begin(), indices.end());

    vector<Point<long double>> points;
    vector<vector<int>> on(n), adj_edges;
    for (auto &[p, indices] : all) {
        sort(indices.begin(), indices.end());
        indices.erase(unique(indices.begin(), indices.end()), indices.end());

        points.emplace_back(p);
        adj_edges.emplace_back();
        for (int i : indices) on[i].emplace_back(points.size() - 1);
    }

    vector<double> edge_angle;
    vector<array<int, 2>> edges;
    set<pair<int, int>> added;
    for (int i = 0; i < n; i++)
        for (int j = 0; j + 1 < on[i].size(); j++) {
            int u = on[i][j], v = on[i][j + 1];
            if (added.emplace(minmax(u, v)).second) {
                int e = 2 * edges.size();
                edges.push_back({u, v});
                auto d = lines[i].b - lines[i].a;
                edge_angle.emplace_back(angle(d));
                edge_angle.emplace_back(angle(-d));
                adj_edges[u].emplace_back(e);
                adj_edges[v].emplace_back(e ^ 1);
            }
        }

    int V = points.size(), E = edges.size(), count = 0;
    vector<int> component(V, -1);
    queue<int> q;
    for (int s = 0; s < V; s++) {
        if (component[s] != -1) continue;
        component[s] = count;
        q.emplace(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (int e : adj_edges[v]) {
                int u = edges[e >> 1][(e & 1) ^ 1];

                if (component[u] == -1) {
                    component[u] = count;
                    q.emplace(u);
                }
            }
        }
        count++;
    }

    int habitats = E - V + count;
    if (!habitats) {
        cout << "No ants :(";
        exit(0);
    }

    for (int v = 0; v < V; v++)
        sort(adj_edges[v].begin(), adj_edges[v].end(), [&](int e1, int e2) { return edge_angle[e1] < edge_angle[e2]; });

    vector<int> next(2 * E);
    for (int v = 0; v < V; v++) {
        int d = adj_edges[v].size();
        for (int i = 0; i < d; i++) next[adj_edges[v][i] ^ 1] = adj_edges[v][i ? i - 1 : d - 1];
    }

    vector<vector<Point<long double>>> faces(count);
    vector<bool> visited(2 * E, false);
    for (int e1 = 0; e1 < 2 * E; e1++)
        if (!visited[e1]) {
            vector<Point<long double>> face;
            int e2 = e1;
            do {
                visited[e2] = true;
                face.emplace_back(points[edges[e2 >> 1][e2 & 1]]);
                e2 = next[e2];
            } while (e2 != e1);

            auto a = doubled_signed_area_of_polygon(face);
            if (a < 0) faces[component[edges[e1 >> 1][e1 & 1]]] = face;
        }

    auto total = 0.L;
    for (int i = 0; i < count; i++)
        if (!faces[i].empty()) {
            bool enclosed = false;
            for (int j = 0; j < count && !enclosed; j++)
                if (i != j && !faces[j].empty()) enclosed = point_in_polygon(faces[j], faces[i][0]).first;
            if (!enclosed) total -= doubled_signed_area_of_polygon(faces[i]) / 2;
        }
    cout << fixed << setprecision(6) << total / habitats;
}