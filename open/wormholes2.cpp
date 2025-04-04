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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int c;
    cin >> c;

    while (c--) {
        Point3D<int> src, dest;
        cin >> src.x >> src.y >> src.z >> dest.x >> dest.y >> dest.z;

        int n;
        cin >> n;

        vector<int> t(n + 2, -1e9), d(n + 2, 0);
        vector<array<Point3D<int>, 2>> coords(n + 2);
        coords[0] = {src, src};
        coords[n + 1] = {dest, dest};
        for (int i = 1; i <= n; i++) {
            auto &[p, q] = coords[i];
            cin >> p.x >> p.y >> p.z >> q.x >> q.y >> q.z >> t[i] >> d[i];
        }

        vector<vector<int>> dist(n + 2, vector<int>(n + 2));
        for (int i = 0; i < n + 2; i++)
            for (int j = 0; j < n + 2; j++) dist[i][j] = ceil(euclidean_dist(coords[i][1], coords[j][0]));

        vector<int> dp(n + 2, 1e9), prev(n + 2, -1);
        dp[0] = 0;
        for (;;) {
            for (int i = 0; i < n + 2; i++)
                for (int j = 0; j < n + 2; j++) {
                    int time = max(dp[i] + dist[i][j], t[j]) + d[j];

                    if (dp[j] > time) {
                        dp[j] = time;
                        prev[j] = i;
                        goto next;
                    }
                }
            break;

            next:;
            for (int i = 0; i < n + 2; i++) {
                int j = i, count = 0;
                while (j != -1 && count < n + 2) {
                    j = prev[j];
                    count++;
                }

                if (~j) {
                    int k = prev[j], offset = dp[prev[j]] + dist[prev[j]][j] - t[j];
                    while (k != j) {
                        offset = min(offset, dp[prev[k]] + dist[prev[k]][k] - t[k]);
                        k = prev[k];
                    }
                    if (offset <= 0) continue;

                    k = j;
                    do {
                        dp[k] -= offset;
                        k = prev[k];
                    } while (k != j);
                }
            }
        }
        cout << dp.back() << "\n";
    }
}
