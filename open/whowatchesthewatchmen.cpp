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

template <typename T, typename U>
U hungarian(const vector<vector<T>> &C, const U delta) {
    int n = C.size();

    vector<int> r_match(n, -1), c_match(n, -1);
    vector<T> potential(n, 0);
    for (int c = 0; c < n; c++) {
        int r = 0;
        for (int row = 1; row < n; row++)
            if (C[r][c] > C[row][c]) r = row;

        potential[c] = C[r][c];
        if (r_match[r] == -1) {
            r_match[r] = c;
            c_match[c] = r;
        }
    }

    auto diff = [&](int r, int c) {
        return C[r][c] - potential[c];
    };

    vector<int> cols(n);
    iota(cols.begin(), cols.end(), 0);
    for (int r = 0; r < n; r++) {
        if (r_match[r] != -1) continue;

        vector<T> dist(n);
        for (int c = 0; c < n; c++) dist[c] = diff(r, c);

        vector<int> prev(n, r);
        int scanned = 0, labeled = 0, last = 0, curr;
        for (;;) {
            if (scanned == labeled) {
                auto d = dist[cols[scanned]];
                for (int c = scanned; c < n; c++)
                    if (dist[cols[c]] <= d) {
                        if (dist[cols[c]] < d) {
                            d = dist[cols[c]];
                            labeled = scanned;
                        }

                        swap(cols[c], cols[labeled++]);
                    }

                for (int c = scanned; c < labeled; c++)
                    if (c_match[cols[c]] == -1) {
                        curr = cols[c];
                        goto done;
                    }

                last = scanned;
            }

            int c1 = cols[scanned++], r1 = c_match[c1];
            for (int c = labeled; c < n; c++) {
                int c2 = cols[c];
                auto d = diff(r1, c2) - diff(r1, c1);

                if (dist[c2] > dist[c1] + d) {
                    dist[c2] = dist[c1] + d;
                    prev[c2] = r1;

                    if (!d) {
                        if (c_match[c2] == -1) {
                            curr = c2;
                            goto done;
                        }

                        swap(cols[c], cols[labeled++]);
                    }
                }
            }
        }

        done:;
        for (int c = 0; c < last; c++) potential[cols[c]] += dist[cols[c]] - dist[curr];
        int temp = curr;
        while (temp != -1) {
            curr = temp;
            c_match[curr] = prev[curr];
            swap(r_match[prev[curr]], temp);
        }
    }

    U cost = 0;
    for (int r = 0; r < n; r++) cost += C[r][r_match[r]];
    return cost - delta;
}

template <typename T>
bool approximately_equal(const T &v1, const T &v2, double epsilon = 1e-5) {
    return fabs(v1 - v2) <= epsilon;
}

template <typename T>
int sgn(const T &v) {
    return approximately_equal(v, (T) 0) ? 0 : (v > 0) - (v < 0);
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
T dot(const Point3D<T> &a, const Point3D<T> &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <typename T>
double norm(const Point3D<T> &p) {
    return sqrt(dot(p, p));
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
bool coplanar(const Point3D<T> &a, const Point3D<T> &b, const Point3D<T> &c, const Point3D<T> &d) {
    if (collinear(a, b, c)) return true;
    auto p = cross(a, b, c);
    return sgn(abs(dot(p, d - a)) / norm(p) - max({norm(b - a), norm(c - a), norm(d - a)})) <= 0;
}

template <typename T>
Point3D<T> normalize(const Point3D<T> &p) {
    T g = __gcd(abs(p.x), __gcd(abs(p.y), abs(p.z)));
    if (!sgn(g)) return p;
    return p / g;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    if (n == 1) {
        cout << -1;
        exit(0);
    }

    vector<Point3D<long long>> sentries(n), dirs(n);
    for (int i = 0; i < n; i++) {
        int x, y, z, vx, vy, vz;
        cin >> x >> y >> z >> vx >> vy >> vz;

        sentries[i] = {x, y, z};
        dirs[i] = normalize(Point3D<long long>{vx, vy, vz});
    }

    auto v = normalize(sentries[1] - sentries[0]);
    if (n & 1 && adjacent_find(sentries.begin() + 1, sentries.end(), [&](const auto &a, const auto &b) {
        auto u = normalize(b - a);
        return u != v && u != -v;
    }) == sentries.end()) {
        vector<int> order(n);
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int i, int j) {
            return dot(v, sentries[i]) < dot(v, sentries[j]);
        });

        vector<Point3D<long long>> s(n), d(n);
        vector<int> state(n, 0);
        for (int i = 0; i < n; i++) {
            s[i] = sentries[order[i]];
            d[i] = dirs[order[i]];

            if (d[i] == v) state[i] = 1;
            else if (d[i] == -v) state[i] = 2;
        }

        auto need_rotate = [&](auto x, auto y, auto z) {
            if (!coplanar({0, 0, 0}, x, y, z) ||
                dot(cross(x, y), cross(x, z)) >= 0 ||
                dot(cross(y, x), cross(y, z)) >= 0 ||
                dot(cross(z, x), cross(z, y)) >= 0) return true;
            return false;
        };

        int energy = INT_MAX;
        for (int i = 0; i < n; i++) {
            vector<Point3D<long long>> temp_s, temp_d;
            vector<int> temp_state;
            for (int j = 0; j < n; j++)
                if (j != i) {
                    temp_s.emplace_back(s[j]);
                    temp_d.emplace_back(d[j]);
                    temp_state.emplace_back(state[j]);
                }

            vector<int> pref1(n), pref2(n), pref3(n);
            for (int j = 0; j < n - 1; j++) {
                pref1[j + 1] = pref1[j] + (temp_state[j] != ((j & 1) + 1));
                pref2[j + 1] = pref2[j] + (temp_state[j] != 1);
                pref3[j + 1] = pref3[j] + (temp_state[j] != 2);
            }
            pref3.erase(pref3.begin());

            for (int ml = 0; ml < n - 1; ml += 2)
                for (int mr = ml + 1; mr < n - 1; mr += 2) {
                    int e1 = pref1[ml] + (pref1[n - 1] - pref1[mr + 1]),
                        e2 = pref2[mr] - pref2[ml] + ((!temp_state[mr] && !state[i]) ? need_rotate(temp_d[mr], d[i], temp_s[mr] - temp_s[ml]) : (!!temp_state[mr] + !!state[i])),
                        e3 = pref3[mr] - pref3[ml] + ((!temp_state[ml] && !state[i]) ? need_rotate(temp_d[ml], d[i], temp_s[ml] - temp_s[mr]) : (!!temp_state[ml] + !!state[i]));
                    energy = min({energy, e1 + e2, e1 + e3});
                }
        }
        cout << energy + 1000;
    } else {
        vector<vector<int>> cost(n, vector<int>(n, 1e9));
        for (int i = 0; i < n; i++) {
            unordered_map<Point3D<long long>, int, Hash> indices;
            for (int j = 0; j < n; j++)
                if (j != i) {
                    auto u = normalize(sentries[j] - sentries[i]);
                    if (!indices.count(u) || norm(sentries[i] - sentries[indices[u]]) > norm(sentries[i] - sentries[j])) indices[u] = j;
                }

            for (auto [u, j] : indices) cost[i][j] = dirs[i] != u;
        }
        cout << hungarian(cost, 0);
    }
}
