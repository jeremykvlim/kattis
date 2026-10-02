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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<Point3D<int>>> points(4);
    for (int z = 0; z < n; z++) {
        char _;
        cin >> _;

        for (int y = 0; y < n; y++) {
            string s;
            cin >> s;

            auto index = [&](char c) {
                if (c == 'B') return 0;
                if (c == 'A') return 1;
                if (c == 'P') return 2;
                return 3;
            };
            for (int x = 0; x < n; x++) points[index(s[x])].emplace_back(x, y, z);
        }
    }

    unordered_map<Point3D<int>, vector<pair<int, int>>, Hash> dots;
    for (auto A : points[1])
        for (auto P : points[2]) {
            auto v = P - A;
            v /= gcd(abs(v.x), gcd(abs(v.y), abs(v.z)));
            if ((v.x ? v.x : v.y ? v.y : v.z) < 0) v = -v;
            dots[v].emplace_back(dot(v, A), dot(v, P));
        }

    auto count = 0LL;
    for (auto &[v, d] : dots) {
        int l = (n - 1) * (min(v.x, 0) + min(v.y, 0) + min(v.z, 0)), r = (n - 1) * (max(v.x, 0) + max(v.y, 0) + max(v.z, 0));
        vector<int> count_B(r - l + 1, 0), count_C(r - l + 1, 0);
        for (auto B : points[0]) count_B[dot(v, B) - l]++;
        for (auto C : points[3]) count_C[dot(v, C) - l]++;
        for (auto [da, dp] : d) count += count_B[da - l] * count_C[dp - l];
    }
    cout << count;
}
