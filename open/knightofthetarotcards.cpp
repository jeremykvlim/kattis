#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;

struct Hash {
    template <typename T>
    static inline void combine(size_t &h, const T &v) {
        h ^= Hash{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }

    template <typename T>
    size_t operator()(const T &v) const {
        if constexpr (requires {tuple_size<T>::value;})
            return apply([](const auto &...e) {
                size_t h = 0;
                (combine(h, e), ...);
                return h;
            }, v);
        else if constexpr (requires {declval<T>().begin(); declval<T>().end();} && !is_same_v<T, string>) {
            size_t h = 0;
            for (const auto &e : v) combine(h, e);
            return h;
        } else return hash<T>{}(v);
    }
};

template <typename T>
tuple<T, T, T> extended_gcd(const T &a, const T &b) {
    if (b == (T) 0) return {a, (T) 1, (T) 0};

    auto divmod = [&](const T &x, const T &y) -> pair<T, T> {
        auto div = [&](const T &x, const T &y) {
            auto numer = x * conj(y);
            auto denom = norm(y);
            auto round_div = [&](auto part) {
                return (part >= 0) ? (part + denom / 2) / denom : (part - denom / 2) / denom;
            };
            return complex<long long>(round_div(numer.real()), round_div(numer.imag()));
        };
        auto q = div(x, y), r = x - q * y;
        return {q, r};
    };

    auto [q, r] = divmod(a, b);
    auto [g, s, t] = extended_gcd(b, r);
    return {g, t, s - t * q};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    auto normalize = [&](complex<long long> &Z) {
        if (Z == complex<long long>{0, 0}) return Z;
        if (Z.real() < 0 || !Z.real() && Z.imag() < 0) Z = -Z;
        return Z;
    };

    vector<complex<long long>> pos(n), gcds(n);
    vector<int> p(n);
    for (int i = 0; i < n; i++) {
        long long r, c, a, b;
        cin >> r >> c >> a >> b >> p[i];

        pos[i] = {r, c};
        auto [g, s, t] = extended_gcd(complex<long long>{a, b}, complex<long long>{b, a});
        gcds[i] = normalize(g);
    }

    if (!pos[0].real() && !pos[0].imag()) {
        cout << 0;
        exit(0);
    }

    gp_hash_table<pair<long long, long long>, long long, Hash> dist;
    priority_queue pq(
            [&](const auto &p1, const auto &p2) -> bool {
                return p1.first != p2.first ? p1.first > p2.first
                                            : p1.second.real() != p2.second.real() ? p1.second.real() < p2.second.real()
                                                                                   : p1.second.imag() < p2.second.imag();
            }, vector<pair<long long, complex<long long>>>());

    auto relax = [&](const complex<long long> &Z, long long d) -> void {
        pair<long long, long long> v{Z.real(), Z.imag()};
        if (dist.find(v) == dist.end() || dist[v] > d) {
            dist[v] = d;
            pq.emplace(d, Z);
        }
    };

    vector<complex<long long>> delta(n);
    for (int i = 0; i < n; i++) {
        if (pos[i] == pos[0]) relax(gcds[i], p[i]);
        if (i) delta[i] = pos[i] - pos[0];
    }

    auto divides = [&](const complex<long long> &Z1, const complex<long long> &Z2) {
        auto N = norm(Z1);
        return !((Z2.real() * Z1.real() + Z2.imag() * Z1.imag()) % N) && !((Z2.imag() * Z1.real() - Z2.real() * Z1.imag()) % N);
    };

    while (!pq.empty()) {
        auto [d, v] = pq.top();
        pq.pop();

        if (dist[{v.real(), v.imag()}] != d) continue;

        if (divides(v, pos[0])) {
            cout << d;
            exit(0);
        }

        for (int i = 1; i < n; i++)
            if (divides(v, delta[i])) {
                auto [g, s, t] = extended_gcd(v, gcds[i]);
                relax(normalize(g), d + p[i]);
            }
    }
    cout << -1;
}
