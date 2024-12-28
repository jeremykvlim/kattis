#include <bits/stdc++.h>
using namespace std;

template <typename T, typename U, typename V>
T mul(U x, V y, T mod) {
    return (unsigned __int128) x * y % mod;
}

template <typename T, typename U>
T pow(T base, U exponent, T mod) {
    T value = 1;
    while (exponent) {
        if (exponent & 1) value = mul(value, base, mod);
        base = mul(base, base, mod);
        exponent >>= 1;
    }
    return value;
}

bool isprime(unsigned long long n) {
    if (n < 2) return false;
    if (n == 2 || n == 5 || n == 11) return true;
    if (n % 6 % 4 != 1) return (n | 1) == 3;

    auto miller_rabin = [&](int a) {
        int s = __builtin_ctzll(n - 1);
        auto d = n >> s, x = pow(a % n, d, n);
        if (x == 1 || x == n - 1) return true;

        while (s--) {
            x = mul(x, x, n);
            if (x == n - 1) return true;
        }
        return false;
    };
    if (!miller_rabin(2) || !miller_rabin(3)) return false;

    auto lucas_pseudoprime = [&]() {
        auto normalize = [&](__int128 &x) {
            if (x < 0) x += ((-x / n) + 1) * n;
        };

        __int128 D = -3;
        for (;;) {
            D += D > 0 ? 2 : -2;
            D *= -1;

            int jacobi = 1;
            auto jacobi_symbol = [&](__int128 n) {
                auto a = D;
                normalize(a);

                while (a) {
                    while (!(a & 1)) {
                        a >>= 1;
                        if ((n & 7) == 3 || (n & 7) == 5) jacobi = -jacobi;
                    }
                    if ((a & 3) == 3 && (n & 3) == 3) jacobi = -jacobi;

                    swap(a, n);
                    a %= n;
                }
                return n == 1;
            };
            if (!jacobi_symbol(n)) return false;
            if (jacobi == -1) break;
        }

        string bits;
        auto temp = n + 1;
        while (temp) {
            bits += (temp & 1) ? '1' : '0';
            temp >>= 1;
        }
        bits.pop_back();
        reverse(bits.begin(), bits.end());

        auto div2mod = [&](__int128 x) -> unsigned long long {
            if (x & 1) x += n;
            normalize(x >>= 1);

            return x % n;
        };

        __int128 U = 1, V = 1;
        for (char b : bits) {
            auto U_2k = mul(U, V, n), V_2k = div2mod(mul(V, V, n) + D * mul(U, U, n));

            if (b == '0') {
                U = U_2k;
                V = V_2k;
            } else {
                U = div2mod(U_2k + V_2k);
                V = div2mod(D * U_2k + V_2k);
            }
        }

        return !U;
    };
    return lucas_pseudoprime();
}

template <typename M>
struct ModInt {
    using T = typename decay<decltype(M::value)>::type;

    T value;
    static bool prime_mod;

    static void init() {
        prime_mod = mod() == 998244353 || mod() == (unsigned long long) 1e9 + 7 || mod() == (unsigned long long) 1e9 + 9 || mod() == (unsigned long long) 1e6 + 69 || mod() == 2524775926340780033 || isprime(mod());
    }

    constexpr ModInt() : value() {}

    template <typename U>
    ModInt(const U &x) {
        value = normalize(x);
    }

    template <typename U>
    static T normalize(const U &x) {
        U v = x;
        if (!(-mod() <= x && x < mod())) v = x % mod();
        return v < 0 ? v + mod() : v;
    }

    const T & operator()() const {
        return value;
    }

    template <typename U>
    explicit operator U() const {
        return (U) value;
    }

    constexpr static T mod() {
        return M::value;
    }

    inline auto & operator+=(const ModInt &v) {
        if ((long long) (value += v.value) >= mod()) value -= mod();
        return *this;
    }

    inline auto & operator-=(const ModInt &v) {
        if ((long long) (value -= v.value) < 0) value += mod();
        return *this;
    }

    template <typename U>
    inline auto & operator+=(const U &v) {
        return *this += ModInt(v);
    }

    template <typename U>
    inline auto & operator-=(const U &v) {
        return *this -= ModInt(v);
    }

    auto & operator++() {
        return *this += 1;
    }

    auto & operator--() {
        return *this -= 1;
    }

    auto operator++(int) {
        return *this += 1;
    }

    auto operator--(int) {
        return *this -= 1;
    }

    auto operator-() const {
        return (ModInt) 0 - *this;
    }

    template <typename U = M>
    typename enable_if<is_same<typename ModInt<U>::T, unsigned int>::value, ModInt>::type &operator*=(const ModInt &v) {
        value = normalize((unsigned long long) value * v.value);
        return *this;
    }

    template <typename U = M>
    typename enable_if<is_same<typename ModInt<U>::T, unsigned long long>::value, ModInt>::type &operator*=(const ModInt &v) {
        value = normalize(mul(value, v.value, mod()));
        return *this;
    }

    template <typename U = M>
    typename enable_if<!is_integral<typename ModInt<U>::T>::value, ModInt>::type &operator*=(const ModInt &v) {
        value = normalize(value * v.value);
        return *this;
    }

    auto & operator/=(const ModInt &v) {
        return *this *= inv(v);
    }

    ModInt inv(const ModInt &v) {
        if (prime_mod) {
            ModInt inv = 1, base = v;
            T n = mod() - 2;
            while (n) {
                if (n & 1) inv *= base;
                base *= base;
                n >>= 1;
            }
            return inv;
        }

        T x = 0, y = 1, a = v.value, m = mod();
        while (a) {
            T t = m / a;
            m -= t * a;
            swap(a, m);
            x -= t * y;
            swap(x, y);
        }

        return (ModInt) x;
    }
};

template <typename T>
bool operator==(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return lhs.value == rhs.value;
}

template <typename T, typename U>
bool operator==(const ModInt<T> &lhs, U rhs) {
    return lhs == ModInt<T>(rhs);
}

template <typename T, typename U>
bool operator==(U lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) == rhs;
}

template <typename T>
bool operator!=(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return !(lhs == rhs);
}

template <typename T, typename U>
bool operator!=(const ModInt<T> &lhs, U rhs) {
    return !(lhs == rhs);
}

template <typename T, typename U>
bool operator!=(U lhs, const ModInt<T> &rhs) {
    return !(lhs == rhs);
}

template <typename T>
bool operator>(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return lhs.value > rhs.value;
}

template <typename T>
bool operator<(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return lhs.value < rhs.value;
}

template <typename T>
ModInt<T> operator+(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) += rhs;
}

template <typename T, typename U>
ModInt<T> operator+(const ModInt<T> &lhs, U rhs) {
    return ModInt<T>(lhs) += rhs;
}

template <typename T, typename U>
ModInt<T> operator+(U lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) += rhs;
}

template <typename T>
ModInt<T> operator-(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) -= rhs;
}

template <typename T, typename U>
ModInt<T> operator-(const ModInt<T> &lhs, U rhs) {
    return ModInt<T>(lhs) -= rhs;
}

template <typename T, typename U>
ModInt<T> operator-(U lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) -= rhs;
}

template <typename T>
ModInt<T> operator*(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) *= rhs;
}

template <typename T, typename U>
ModInt<T> operator*(const ModInt<T> &lhs, U rhs) {
    return ModInt<T>(lhs) *= rhs;
}

template <typename T, typename U>
ModInt<T> operator*(U lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) *= rhs;
}

template <typename T>
ModInt<T> operator/(const ModInt<T> &lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) /= rhs;
}

template <typename T, typename U>
ModInt<T> operator/(const ModInt<T> &lhs, U rhs) {
    return ModInt<T>(lhs) /= rhs;
}

template <typename T, typename U>
ModInt<T> operator/(U lhs, const ModInt<T> &rhs) {
    return ModInt<T>(lhs) /= rhs;
}

template <typename U, typename T>
U & operator<<(U &stream, const ModInt<T> &v) {
    return stream << v();
}

template <typename U, typename T>
U & operator>>(U &stream, ModInt<T> &v) {
    typename common_type<typename ModInt<T>::T, long long>::type x;
    stream >> x;
    v = ModInt<T>(x);
    return stream;
}

template <typename M>
bool ModInt<M>::prime_mod;

constexpr unsigned long long MODULO = 1e10 + 3233;
using modint = ModInt<integral_constant<decay<decltype(MODULO)>::type, MODULO>>;

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    template <typename U>
    Point(const Point<U> &p) : x((T) p.x), y((T) p.y) {}

    auto operator<(const Point &p) const {
        return x != p.x ? x < p.x : y < p.y;
    }

    auto operator==(const Point &p) const {
        return x == p.x && y == p.y;
    }

    Point operator+(Point p) const {
        return {x + p.x, y + p.y};
    }

    Point operator-(Point p) const {
        return {x - p.x, y - p.y};
    }

    Point operator*(T c) const {
        return {c * x, c * y};
    }

    Point & operator*=(T c) {
        x *= c;
        y *= c;
        return *this;
    }
};

template <typename T>
T cross(Point<T> a, Point<T> b) {
    return (a.x * b.y) - (a.y * b.x);
}

template <typename T>
T area_of_parallelogram(Point<T> a, Point<T> b, Point<T> c) {
    Point<T> u = b - a, v = c - a;
    return abs(cross(u, v));
}

template <typename T>
vector<Point<T>> minkowski(vector<Point<T>> &P, vector<Point<T>> &Q) {
    auto reorder = [&](auto &polygon) {
        int i = min_element(polygon.begin(), polygon.end()) - polygon.begin();
        rotate(polygon.begin(), polygon.begin() + i, polygon.end());
    };
    reorder(P);
    reorder(Q);

    vector<Point<T>> edges;
    for (int i = 0; i < P.size(); i++) edges.emplace_back(P[(i + 1) % P.size()] - P[i]);
    for (int i = 0; i < Q.size(); i++) edges.emplace_back(Q[(i + 1) % Q.size()] - Q[i]);
    sort(edges.begin(), edges.end(), [&](auto a, auto b) {
        auto quadrant = [](const auto &p) -> int {
            if (p.x > 0) return 1;
            if (!p.x && p.y > 0) return 2;
            if (p.x < 0) return 3;
            if (!p.x && p.y < 0) return 4;
            return 0;
        };

        int qa = quadrant(a), qb = quadrant(b);
        return qa != qb ? qa < qb : cross(a, b) > 0;
    });

    vector<Point<T>> m{P[0] + Q[0]};
    for (int i = 0; i < edges.size() - 1; i++) m.emplace_back(m.back() + edges[i]);
    return m;
}

template <typename T>
T distance_between_polygons(vector<Point<T>> P, vector<Point<T>> Q) {
    for (auto &p : Q) p *= -1;

    auto m = minkowski(P, Q);

    T dist = 0;
    for (int i = 0; i < m.size(); i++) dist += area_of_parallelogram({0, 0}, m[i], m[(i + 1) % m.size()]) - area_of_parallelogram(m[0], m[i], m[(i + 1) % m.size()]);
    return dist;
}

template <typename T>
T cross(Point<T> a, Point<T> b, Point<T> c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

template <typename T>
vector<Point<T>> monotone_chain(vector<Point<T>> points, bool collinear = false) {
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    if (points.size() < 3) return points;

    vector<Point<T>> convex_hull;

    auto clockwise = [&](auto p) {
        auto cross_product = cross(convex_hull[convex_hull.size() - 2], convex_hull.back(), p);
        return collinear ? cross_product <= 0 : cross_product < 0;
    };

    for (auto p : points) {
        while (convex_hull.size() > 1 && clockwise(p)) convex_hull.pop_back();
        convex_hull.emplace_back(p);
    }

    int s = convex_hull.size();
    points.pop_back();
    reverse(points.begin(), points.end());
    for (auto p : points) {
        while (convex_hull.size() > s && clockwise(p)) convex_hull.pop_back();
        convex_hull.emplace_back(p);
    }

    convex_hull.pop_back();
    return convex_hull;
}

template <typename T>
struct FlowNetwork {
    struct Arc {
        int u, v;
        T cap;
        Arc(int u, int v, T cap) : u(u), v(v), cap(cap) {}
    };

    int n;
    vector<vector<Arc>> network;
    FlowNetwork(int n) : n(n), network(n) {}

    void add_arc(int u, int v, T cap_uv, T cap_vu = 0) {
        if (u == v) return;

        network[u].emplace_back(v, network[v].size(), cap_uv);
        network[v].emplace_back(u, network[u].size() - 1, cap_vu);
    }

    T max_flow(int s, int t) {
        if (s == t) return 0;

        vector<T> excess(n, 0);
        vector<int> height(n, 0), height_count(2 * n, 0);
        vector<typename vector<Arc>::iterator> curr(n);
        excess[t] = 1;
        height[s] = n;
        height_count[0] = n - 1;
        for (int v = 0; v < n; v++) curr[v] = network[v].begin();

        vector<vector<int>> active(2 * n);
        auto push = [&](int v, Arc &a, T delta) {
            int u = a.u;
            if (!abs(excess[u]) && delta > 0) active[height[u]].emplace_back(u);
            a.cap -= delta;
            network[u][a.v].cap += delta;
            excess[v] -= delta;
            excess[u] += delta;
        };

        for (auto &&a : network[s]) push(s, a, a.cap);

        if (!active[0].empty())
            for (int h = 0; h >= 0;) {
                int v = active[h].back();
                active[h].pop_back();

                while (excess[v] > 0)
                    if (curr[v] == network[v].end()) {
                        height[v] = INT_MAX;

                        for (auto a = network[v].begin(); a != network[v].end(); a++)
                            if (a->cap > 0 && height[v] > height[a->u] + 1) height[v] = height[(curr[v] = a)->u] + 1;

                        height_count[height[v]]++;
                        if (!--height_count[h] && h < n)
                            for (int w = 0; w < n; w++)
                                if (h < height[w] && height[w] < n) {
                                    height_count[height[w]]--;
                                    height[w] = n + 1;
                                }
                        h = height[v];
                    } else if (curr[v]->cap > 0 && height[v] == height[curr[v]->u] + 1) push(v, *curr[v], min(excess[v], curr[v]->cap));
                    else curr[v]++;

                while (h >= 0 && active[h].empty()) h--;
            }

        return -excess[s];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    modint::init();

    int h, n;
    cin >> h >> n;

    vector<long long> c(n);
    vector<Point<long long>> o(n);
    vector<vector<Point<long long>>> convex_hulls(n);
    for (int i = 0; i < n; i++) {
        int k;
        cin >> k >> c[i] >> o[i].x >> o[i].y;

        vector<Point<long long>> v(k);
        for (auto &[x, y] : v) cin >> x >> y;
        v.emplace_back(0, 0);

        c[i] /= 2;
        convex_hulls[i] = monotone_chain(v);
    }

    int m;
    cin >> m;

    vector<long long> F(m + 1, 0);
    for (int i = 1; i <= m; i++) {
        cin >> F[i];

        F[i] += F[i - 1];
    }

    auto s = [&](int i, auto f) {
        vector<Point<__int128>> S(convex_hulls[i].size());
        for (int j = 0; j < convex_hulls[i].size(); j++) S[j] = o[i] + convex_hulls[i][j] * f;
        return S;
    };

    vector<vector<pair<int, int>>> time(m + 2);
    for (int i = 0; i < n; i++) {
        auto search = [&](auto Y) {
            int l = 0, r = m + 1, mid;
            while (l + 1 < r) {
                mid = l + (r - l) / 2;

                bool less = false, more = false;
                for (auto [x, y] : s(i, F[mid])) {
                    if (y <= Y) less = true;
                    if (y >= Y) more = true;
                }

                if (less && more) r = mid;
                else l = mid;
            }
            return r;
        };
        time[search(-h)].emplace_back(i, n);
        time[search(h)].emplace_back(i, n + 1);

        for (int j = i + 1; j < n; j++) {
            int l = 0, r = m + 1, mid;
            while (l + 1 < r) {
                mid = l + (r - l) / 2;

                if (!distance_between_polygons(s(i, F[mid]), s(j, F[mid]))) r = mid;
                else l = mid;
            }
            time[r].emplace_back(i, j);
        }
    }

    for (int i = 0; i < n; i++) {
        auto S = s(i, 1);

        auto sum = 0LL;
        for (int j = 0; j < S.size(); j++) sum += area_of_parallelogram(S[0], S[j], S[(j + 1) % S.size()]);
        c[i] *= sum;
    }

    FlowNetwork<long long> fn(2 * n + 2);
    for (int i = 0; i < n; i++) fn.add_arc(i, i + n, c[i]);

    modint cost = 0;
    for (int t = 1; t <= m; t++) {
        for (auto [i, j] : time[t])
            if (j == n) fn.add_arc(2 * n, i, 1e18);
            else if (j == n + 1) fn.add_arc(i + n, 2 * n + 1, 1e18);
            else {
                fn.add_arc(i + n, j, 1e18);
                fn.add_arc(j + n, i, 1e18);
            }

        if (!time[t].empty()) cost += fn.max_flow(2 * n, 2 * n + 1);
        cout << cost * F[t] * F[t] << "\n";
    }
}
