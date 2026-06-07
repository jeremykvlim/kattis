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
array<T, 3> extended_gcd(const T &a, const T &b) {
    if (b == (T) 0) return {a, (T) 1, (T) 0};

    auto div = [&](const T &x, const T &y) {
        if constexpr (!requires(T z) { z.real(); z.imag(); }) return x / y;
        else {
            T numer = x * conj(y);
            auto denom = norm(y);
            auto round_div = [&](auto part) {
                return (part >= 0) ? (part + denom / 2) / denom : (part - denom / 2) / denom;
            };
            return (T) {round_div(numer.real()), round_div(numer.imag())};
        }
    };

    T q = div(a, b), r = a - q * b;
    auto [g, s, t] = extended_gcd(b, r);
    return {g, t, s - t * q};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, sr, sc, sa, sb, sp;
    cin >> n >> sr >> sc >> sa >> sb >> sp;

    if (!sr && !sc) {
        cout << 0;
        exit(0);
    }

    vector<array<int, 3>> start;
    vector<array<int, 5>> other;
    auto add = [&](int r, int c, int a, int b, int p) {
        complex<long long> Z{a, b};
        auto [G, _, __] = extended_gcd(Z, conj(Z));

        int real = abs(G.real()), imag = abs(G.imag()), g, t;
        if (!real || !imag) {
            g = real ^ imag;
            t = 1;
        } else {
            g = gcd(real, imag);
            t = 2;
        }

        if (r == sr && c == sc) start.push_back({p, g, t});
        else other.push_back({r - sr, c - sc, p, g, t});
    };
    add(sr, sc, sa, sb, sp);

    for (int _ = 0; _ < n - 1; _++) {
        int r, c, a, b, p;
        cin >> r >> c >> a >> b >> p;

        add(r, c, a, b, p);
    }

    auto cost = LLONG_MAX;
    vector<long long> dist;
    vector<array<int, 2>> states;
    unordered_map<array<int, 2>, int, Hash> indices;
    auto relax = [&](long long d, int g, int t) {
        if (cost <= d) return false;

        auto [it, inserted] = indices.emplace(array{g, t}, states.size());
        int i = it->second;
        if (inserted) {
            dist.emplace_back(d);
            states.push_back({g, t});
            return true;
        }

        if (dist[i] > d) {
            dist[i] = d;
            return true;
        }
        return false;
    };

    auto reachable = [&](int g, int t) {
        return !(sr % g) && !(sc % g) && !((sr + sc) % (g * t));
    };

    for (auto &[p, g1, t1] : start) {
        int s = states.size(), z1 = countr_zero((unsigned) g1);
        if (reachable(g1, t1)) cost = min(cost, (long long) p);
        relax(p, g1, t1);

        for (int i = 0; i < s; i++) {
            auto d = dist[i] + p;
            if (cost > d) {
                auto [g2, t2] = states[i];
                int z2 = countr_zero((unsigned) g2), g3 = gcd(g1, g2), t3 = min(z1 + (t1 == 2), z2 + (t2 == 2)) > min(z1, z2) ? 2 : 1;
                if (g2 != g3 || t2 != t3) {
                    if (reachable(g3, t3)) cost = d;
                    relax(d, g3, t3);
                }
            }
        }
    }

    priority_queue<tuple<long long, int, int>, vector<tuple<long long, int, int>>, greater<>> pq;
    for (int i = 0; i < states.size(); i++)
        if (cost > dist[i]) pq.emplace(dist[i], states[i][0], states[i][1]);

    while (!pq.empty()) {
        auto [p1, g1, t1] = pq.top();
        pq.pop();

        auto it = indices.find({g1, t1});
        if (it == indices.end() || dist[it->second] != p1) continue;
        if (p1 >= cost) break;

        int z1 = countr_zero((unsigned) g1);
        for (auto &[dr, dc, p2, g2, t2] : other) {
            if (dr % g1 || dc % g1 || (dr + dc) % (g1 * t1)) continue;
            auto d = p1 + p2;
            if (cost > d) {
                int z2 = countr_zero((unsigned) g2), g3 = gcd(g1, g2), t3 = min(z1 + (t1 == 2), z2 + (t2 == 2)) > min(z1, z2) ? 2 : 1;
                if (g1 != g3 || t1 != t3) {
                    if (reachable(g3, t3)) cost = d;
                    if (relax(d, g3, t3)) pq.emplace(d, g3, t3);
                }
            }
        }
    }

    if (cost != LLONG_MAX) cout << cost;
    else cout << -1;
}
