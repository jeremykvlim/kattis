#include <bits/stdc++.h>
using namespace std;

template <typename T>
array<T, 3> extended_gcd(const T &a, const T &b) {
    if (b == (T) 0) return {a, (T) 1, (T) 0};

    T q = a / b, r = a - q * b;
    auto [g, s, t] = extended_gcd(b, r);
    return {g, t, s - t * q};
}

template <typename T>
tuple<T, T, bool> linear_diophantine_solution(T &a, T &b, T c) {
    auto [g, x, y] = extended_gcd(a, b);
    T q = c / g, r = c - q * g;
    if (r != (T) 0) return {x, y, false};

    a /= g;
    b /= g;
    c /= g;
    x *= c;
    y *= c;
    return {x, y, true};
}

template <typename T>
struct GaussianInteger {
    T a, b;

    GaussianInteger(T a = 0, T b = 0) : a(a), b(b) {}

    GaussianInteger operator-() const {
        return {-a, -b};
    }

    GaussianInteger operator~() const {
        return {-b, a};
    }

    GaussianInteger operator+(const GaussianInteger &z) const {
        return {a + z.a, b + z.b};
    }

    GaussianInteger operator-(const GaussianInteger &z) const {
        return {a - z.a, b - z.b};
    }

    GaussianInteger operator*(const GaussianInteger &z) const {
        return {a * z.a - b * z.b, a * z.b + b * z.a};
    }

    GaussianInteger operator/(const GaussianInteger &z) const {
        T N = z.norm(), x = a * z.a + b * z.b, y = b * z.a - a * z.b;
        auto round_div = [N](T a) {
            T q = a / N, r = a % N;
            if (r >= (N + 1) / 2) q++;
            if (r <= -(N + 1) / 2) q--;
            return q;
        };
        return {round_div(x), round_div(y)};
    }

    GaussianInteger operator%(const GaussianInteger &z) const {
        return *this - *this / z * z;
    }

    GaussianInteger & operator+=(const GaussianInteger &z) {
        return *this = *this + z;
    }

    GaussianInteger & operator-=(const GaussianInteger &z) {
        return *this = *this - z;
    }

    GaussianInteger & operator*=(const GaussianInteger &z) {
        return *this = *this * z;
    }

    GaussianInteger & operator/=(const GaussianInteger &z) {
        return *this = *this / z;
    }

    GaussianInteger & operator%=(const GaussianInteger &z) {
        return *this = *this % z;
    }

    bool operator==(const GaussianInteger &) const = default;

    GaussianInteger conj() const {
        return {a, -b};
    }

    T norm() const {
        return a * a + b * b;
    }

    template <typename U>
    static GaussianInteger pow(GaussianInteger base, U exponent) {
        GaussianInteger value = 1;
        while (exponent) {
            if (exponent & 1) value *= base;
            base *= base;
            exponent >>= 1;
        }
        return value;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long a, b, x, y;
    cin >> a >> b >> x >> y;

    GaussianInteger<long long> A{a, b}, B{b, a}, C{x, y};
    auto [X, Y, _] = linear_diophantine_solution(A, B, C);

    auto a1 = A.a, b1 = A.b,
         a2 = B.a, b2 = B.b,
         c1 = X.a, c2 = X.b,
         c3 = Y.a, c4 = Y.b;

    array<array<long long, 3>, 4> lines{{{a2, -b2, -c1}, {b2, a2, -c2}, {-a1, b1, -c3}, {-b1, -a1, -c4}}};

    auto sum = [&](long long u, long long v) {
        return abs(u * a2 - v * b2 + c1) + abs(u * b2 + v * a2 + c2) + abs(-u * a1 + v * b1 + c3) + abs(-u * b1 - v * a1 + c4);
    };

    long long U = 0, V = 0, moves = sum(0, 0);
    for (int i = 0; i < 4; i++) {
        auto [ai, bi, ci] = lines[i];
        for (int j = 0; j < i; j++) {
            auto [aj, bj, cj] = lines[j];
            auto det = ai * bj - aj * bi;
            if (!det) continue;

            auto floor_div = [](__int128 x, __int128 y) -> long long {
                if (y < 0) {
                    x = -x;
                    y = -y;
                }
                return x >= 0 ? x / y : -(-x + y - 1) / y;
            };
            auto u = floor_div((__int128) ci * bj - (__int128) cj * bi, det), v = floor_div((__int128) ai * cj - (__int128) aj * ci, det);
            for (int du = -2; du <= 2; du++)
                for (int dv = -2; dv <= 2; dv++) {
                    auto m = sum(u + du, v + dv);
                    if (moves > m) {
                        moves = m;
                        U = u + du;
                        V = v + dv;
                    }
                }
        }
    }

    bool change;
    do {
        change = false;
        auto u = U, v = V;
        for (int du = -1; du <= 1; du++)
            for (int dv = -1; dv <= 1; dv++) {
                auto m = sum(u + du, v + dv);
                if (moves > m) {
                    moves = m;
                    U = u + du;
                    V = v + dv;
                    change = true;
                }
            }
    } while (change);
    cout << moves;
}
