#include <bits/stdc++.h>
using namespace std;

template <typename T>
pair<T, T> bezout(T a, T b) {
    if (!a) return {0, 1};
    auto [x, y] = bezout(b % a, a);
    return {y - (b / a) * x, x};
}

template <typename T>
T linear_congruence_solution(T a, T b, T n) {
    T g = __gcd(a, n);
    if (b % g) return -1;

    a /= g;
    b /= g;
    n /= g;
    auto [x, y] = bezout(a, n);
    x = (x + n) % n;
    return (b * x) % n;
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
