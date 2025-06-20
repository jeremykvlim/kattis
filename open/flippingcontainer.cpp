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
tuple<T, T, T> extended_gcd(const T &a, const T &b) {
    if (b == (T) 0) return {a, (T) 1, (T) 0};

    auto divmod = [&](const T &x, const T &y) -> pair<T, T> {
        auto q = x / y, r = x - q * y;
        return {q, r};
    };

    auto [q, r] = divmod(a, b);
    auto [g, s, t] = extended_gcd(b, r);
    return {g, t, s - t * q};
}

template <typename T>
tuple<T, T, bool> linear_diophantine_solution(T &a, T &b, T c) {
    auto divmod = [&](const T &x, const T &y) -> pair<T, T> {
        auto q = x / y, r = x - q * y;
        return {q, r};
    };

    auto [g, x, y] = extended_gcd(a, b);
    auto [q, r] = divmod(c, g);
    if (r != (T) 0) return {x, y, false};

    a /= g;
    b /= g;
    c /= g;
    x *= c;
    y *= c;
    return {x, y, true};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int A, B, C;
    long long X, Y;
    cin >> A >> B >> C >> X >> Y;

    vector<int> steps{A + B, A + C, B + C};
    sort(steps.begin(), steps.end());

    unordered_map<int, int> pos;
    for (int i = 0; i < 3; i++) pos[steps[i]] = 1 << i;

    auto flips = LLONG_MAX;
    unordered_map<pair<vector<int>, long long>, long long, Hash> memo;
    unordered_map<array<__int128, 3>, long long, Hash> eqns;
    auto dfs = [&](auto &&self, int a, int b, int c, int dx = 0, int dy = 0, int mx = 0, int my = 0, int f = 0) {
        mx |= pos[a + c];
        my |= pos[b + c];

        auto count = [&](__int128 a, __int128 b, __int128 c) -> long long {
            if (eqns.count({a, b, c})) return eqns[{a, b, c}];

            auto [x, y, valid] = linear_diophantine_solution(a, b, c);
            if (!valid) return eqns[{a, b, c}] = 1e18;

            long long lowest = 1e18, kx = -x / b, ky = y / a;
            for (auto k0 : {kx, ky})
                for (int dk = -1; dk <= 1; dk++) {
                    auto k = k0 + dk;
                    lowest = min(lowest, llabs(x + k * b) + llabs(y - k * a));
                }
            return eqns[{a, b, c}] = lowest;
        };

        auto dir_flips = [&](int mask, long long dist) -> long long {
            vector<int> s;
            for (int i = 0; i < 3; i++)
                if ((mask >> i) & 1) s.emplace_back(steps[i]);

            if (s.empty()) return 1e18;
            if (s.size() == 1) return !(dist % s[0]) ? abs(dist) / s[0] : 1e18;
            if (s.size() == 2) return count(s[0], s[1], dist);
            if (memo.count({s, dist})) return memo[{s, dist}];

            auto f = LLONG_MAX;
            bool l = false, r = false;
            for (int i = 0; i <= s[1] && !l || !r; i++)
                for (int sgn : {1, -1}) {
                    if (!i && sgn == -1) continue;

                    int k = sgn * i;
                    if (k < -s[1]) {
                        l = true;
                        continue;
                    }
                    if (k > s[1]) {
                        r = true;
                        continue;
                    }

                    if (f <= abs(k) + (abs(dist - s[0] * k) + s[2] - 1) / s[2]) {
                        if (k < 0) l = true;
                        if (k > 0) r = true;
                        continue;
                    }
                    f = min(f, abs(k) + count(s[1], s[2], dist - s[0] * k));
                }

            return memo[{s, dist}] = f;
        };

        if (a == A && b == B) {
            auto fx = dir_flips(mx, X - dx), fy = dir_flips(my, Y - dy);
            if (fx < 1e18 && fy < 1e18) flips = min(flips, 2 * (fx + fy) + f);
        }
        if (f == 8) return;

        self(self, a, c, b, dx, dy + b, mx, my, f + 1);
        self(self, a, c, b, dx, dy - c, mx, my, f + 1);
        self(self, c, b, a, dx + a, dy, mx, my, f + 1);
        self(self, c, b, a, dx - c, dy, mx, my, f + 1);
    };
    dfs(dfs, A, B, C);

    cout << (flips == LLONG_MAX ? "impossible" : to_string(flips));
}
