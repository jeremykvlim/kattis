#include <bits/stdc++.h>
using namespace std;

template <typename T>
bool approximately_equal(const T &v1, const T &v2, double epsilon = 1e-5) {
    return fabs(v1 - v2) <= epsilon;
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
};

template <typename T>
double euclidean_dist(const Point3D<T> &a, const Point3D<T> &b = {0, 0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y) + (double) (a.z - b.z) * (a.z - b.z));
}

template <typename T, typename U>
U hungarian(const vector<vector<T>> &adj_matrix, const U delta) {
    int n = adj_matrix.size();

    vector<int> r_match(n, -1), c_match(n, -1);
    vector<T> potential(n, 0);
    for (int c = 0; c < n; c++) {
        int r = 0;
        for (int row = 1; row < n; row++)
            if (adj_matrix[r][c] > adj_matrix[row][c]) r = row;

        potential[c] = adj_matrix[r][c];
        if (r_match[r] == -1) {
            r_match[r] = c;
            c_match[c] = r;
        }
    }

    auto diff = [&](int r, int c) {
        return adj_matrix[r][c] - potential[c];
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
    for (int r = 0; r < n; r++) cost += adj_matrix[r][r_match[r]];
    return cost - delta;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, s, t, q;
    cin >> n >> s >> t >> q;

    vector<Point3D<long long>> hills(n);
    for (auto &[x, y, h] : hills) cin >> x >> y >> h;

    vector<vector<double>> dist(n, vector<double>(n, 1e9));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (hills[i].z < hills[j].z && euclidean_dist(hills[i], hills[j]) < q) dist[i][j] = euclidean_dist(hills[i], hills[j]);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++) dist[j][k] = min(dist[j][k], dist[j][i] + dist[i][k]);

    vector<int> spring(s), town(t);
    for (int &ij : spring) cin >> ij;
    for (int &ij : town) cin >> ij;

    vector<vector<double>> adj_matrix(s, vector<double>(s, 0));
    for (int i = 0; i < t; i++)
        for (int j = 0; j < s; j++) adj_matrix[i][j] = dist[town[i] - 1][spring[j] - 1];

    auto cost = hungarian(adj_matrix, 0.);
    if (cost < 1e9) cout << fixed << setprecision(6) << cost;
    else cout << "IMPOSSIBLE";
}
