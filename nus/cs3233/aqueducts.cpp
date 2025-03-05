#include <bits/stdc++.h>
using namespace std;

template <typename T>
bool approximately_equal(const T &v1, const T &v2) {
    return fabs(v1 - v2) <= 1e-5;
}

template <typename T>
struct Point3D {
    T x, y, z;

    Point3D() {}
    Point3D(T x, T y, T z) : x(x), y(y), z(z) {}

    template <typename U>
    Point3D(const Point3D<U> &p) : x((T) p.x), y((T) p.y), z((T) p.z) {}

    Point3D operator-() const {
        return {-x, -y, -z};
    }

    bool operator<(const Point3D &p) const {
        return !approximately_equal(x, p.x) ? x < p.x : (!approximately_equal(y, p.y) ? y < p.y : z < p.z);
    }

    bool operator>(const Point3D &p) const {
        return !approximately_equal(x, p.x) ? x > p.x : (!approximately_equal(y, p.y) ? y > p.y : z > p.z);
    }

    bool operator==(const Point3D &p) const {
        if constexpr (is_floating_point_v<T>) return approximately_equal(x, p.x) && approximately_equal(y, p.y) && approximately_equal(z, p.z);
        return x == p.x && y == p.y && z == p.z;
    }

    bool operator!=(const Point3D &p) const {
        if constexpr (is_floating_point_v<T>) return !approximately_equal(x, p.x) || !approximately_equal(y, p.y) || !approximately_equal(z, p.z);
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

    struct Hash {
        size_t operator()(Point3D<T> p) const {
            auto h = 0ULL;
            h ^= hash<T>()(p.x) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= hash<T>()(p.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= hash<T>()(p.z) + 0x9e3779b9 + (h << 6) + (h >> 2);
            return h;
        }
    };
};

template <typename T>
double dist(const Point3D<T> &a, const Point3D<T> &b) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y) + (double) (a.z - b.z) * (a.z - b.z));
}

template <typename T, typename U>
U hungarian(const vector<vector<T>> &adj_matrix, const U delta) {
    int R = adj_matrix.size(), C = adj_matrix[0].size();

    vector<int> r_match(R, -1), c_match(C, -1);
    vector<T> potential(C, 0);
    for (int c = 0; c < C; c++) {
        int r = 0;
        for (int row = 1; row < R; row++)
            if (adj_matrix[r][c] > adj_matrix[row][c] + 1e-9) r = row;

        potential[c] = adj_matrix[r][c];
        if (r_match[r] == -1) {
            r_match[r] = c;
            c_match[c] = r;
        }
    }

    auto diff = [&](int r, int c) {
        return adj_matrix[r][c] - potential[c];
    };

    vector<int> cols(C);
    iota(cols.begin(), cols.end(), 0);
    for (int r = 0; r < R; r++) {
        if (r_match[r] != -1) continue;

        vector<T> dist(C);
        for (int c = 0; c < C; c++) dist[c] = diff(r, c);

        vector<int> prev(C, r);
        int scanned = 0, labeled = 0, last = 0, curr;
        for (;;) {
            if (scanned == labeled) {
                auto d = dist[cols[scanned]];
                for (int c = scanned; c < C; c++)
                    if (dist[cols[c]] <= d + 1e-9) {
                        if (dist[cols[c]] < d - 1e-9) {
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
            for (int c = labeled; c < C; c++) {
                int c2 = cols[c];
                auto d = diff(r1, c2) - diff(r1, c1);

                if (dist[c2] > dist[c1] + d) {
                    dist[c2] = dist[c1] + d;
                    prev[c2] = r1;

                    if (approximately_equal(d, (T) 0)) {
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
    for (int r = 0; r < R; r++) cost += adj_matrix[r][r_match[r]];
    return cost - delta;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, s, t, q;
    cin >> n >> s >> t >> q;

    vector<Point3D<long long>> hills(n);
    for (auto &[x, y, h] : hills) cin >> x >> y >> h;

    vector<vector<double>> d(n, vector<double>(n, 1e9));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (hills[i].z < hills[j].z && dist(hills[i], hills[j]) < q) d[i][j] = dist(hills[i], hills[j]);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++) d[j][k] = min(d[j][k], d[j][i] + d[i][k]);

    vector<int> spring(s), town(t);
    for (int &ij : spring) cin >> ij;
    for (int &ij : town) cin >> ij;

    vector<vector<double>> adj_matrix(s, vector<double>(s, 0));
    for (int i = 0; i < t; i++)
        for (int j = 0; j < s; j++) adj_matrix[i][j] = d[town[i] - 1][spring[j] - 1];

    auto cost = hungarian(adj_matrix, 0.);
    if (cost < 1e9) cout << fixed << setprecision(6) << cost;
    else cout << "IMPOSSIBLE";
}
