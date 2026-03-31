#include <bits/stdc++.h>
using namespace std;

template <typename T>
array<T, 3> extended_gcd(const T &a, const T &b) {
    if (b == (T) 0) return {a, (T) 1, (T) 0};

    auto divmod = [&](const T &x, const T &y) -> pair<T, T> {
        if constexpr (requires(T z) { z.real(); z.imag(); }) {
            auto div = [&](const T &x, const T &y) -> T {
                T numer = x * conj(y);
                auto denom = norm(y);
                auto round_div = [&](auto part) {
                    return (part >= 0) ? (part + denom / 2) / denom : (part - denom / 2) / denom;
                };
                return (T) {round_div(numer.real()), round_div(numer.imag())};
            };
            T q = div(x, y), r = x - q * y;
            return {q, r};
        } else {
            T q = x / y, r = x - q * y;
            return {q, r};
        }
    };

    auto [q, r] = divmod(a, b);
    auto [g, s, t] = extended_gcd(b, r);
    return {g, t, s - t * q};
}

template <typename T>
T linear_congruence_solution(T a, T b, T n) {
    T g = __gcd(a, n);
    if (b % g) return -1;

    n /= g;
    auto [_, x, y] = extended_gcd(a / g, n);
    x = (x + n) % n;
    return (b / g * x) % n;
}

ostream & operator<<(ostream &stream, const __int128 &v) {
    if (!v) return stream << 0;
    if (v == numeric_limits<__int128>::min()) return stream << "-170141183460469231731687303715884105728";

    auto temp = v;
    if (v < 0) {
        stream << '-';
        temp = -temp;
    }

    string s;
    while (temp) {
        s += '0' + (temp % 10);
        temp /= 10;
    }
    reverse(s.begin(), s.end());
    return stream << s;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long N, A, B;
    cin >> N >> A >> B;

    auto g = __gcd(A, B);
    if (N % g) {
        cout << -1;
        exit(0);
    }

    __int128 n = N / g, a = A / g, b = B / g;
    if (a == 1 || b == 1) {
        cout << N;
        exit(0);
    }

    auto x1 = linear_congruence_solution(b, n % a, a), x2 = linear_congruence_solution(a, n % b, b);
    auto measure = [&](auto c, auto d, auto x) -> __int128 {
        auto q = (n + d - 1) / d;
        if (q > x) x += ((q - x + c - 1) / c) * c;
        return d * g * x;
    };
    auto time = min(measure(a, b, x1), measure(b, a, x2));
    if ((x1 != -1 && x1 * b <= n) || (x2 != -1 && x2 * a <= n)) time = min(time, (__int128) N);
    cout << time;
}
