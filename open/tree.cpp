#include <bits/stdc++.h>
using namespace std;

struct Hash {
    template <typename T>
    static inline void combine(size_t &seed, const T &v) {
        seed ^= Hash{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    template <typename ... Ts>
    static size_t seed_value(const Ts & ... args) {
        size_t seed = 0;
        (combine(seed, args), ...);
        return seed;
    }

    template <typename T>
    size_t operator()(const T &v) const {
        if constexpr (requires {tuple_size<T>::value;})
            return apply([](const auto & ... elems) {return seed_value(elems...);}, v);
        else if constexpr (requires {declval<T>().begin(); declval<T>().end();} && !is_same_v<T, string>) {
            size_t seed = 0;
            for (const auto &e : v) combine(seed, e);
            return seed;
        } else return hash<T>{}(v);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    array<string, 3> routines;
    for (auto &s : routines) cin >> s;

    vector<int> calls{0, 0, 1, 1, 2, 2};
    vector<string> orders{"Pre", "In", "Post"};
    do {
        unordered_map<array<string, 3>, array<string, 3>, Hash> cache;
        auto construct = [&](auto &&self, array<string, 3> r) -> array<string, 3> {
            if (cache.count(r)) return cache[r];

            int n = r[0].size() | r[1].size() | r[2].size();

            bool diff = false;
            auto change = [&](auto &s1, auto s2) {
                diff |= !s1.empty() && s1 != s2;
                s1 = s2;
            };

            if (n == 1) {
                auto s = !r[0].empty() ? r[0] : !r[1].empty() ? r[1] : r[2];
                change(r[0], s);
                change(r[1], s);
                change(r[2], s);

                if (!diff) return r;
                else return {};
            }

            array<string, 3> t;
            for (int i = 0; i < n; i++) {
                auto v = !r[0].empty() ? r[0][0] : !r[1].empty() ? r[1][i] : r[2][n - 1];

                array<string, 3> cl, cr;
                diff = (!r[1].empty() && r[1][i] != v) || (!r[2].empty() && r[2][n - 1] != v);
                for (int j = 2; ~j; j--)
                    if (!r[j].empty()) {
                        change(cl[calls[j << 1]], r[j].substr(!j, i));
                        change(cr[calls[j << 1 | 1]], r[j].substr(i + (j < 2), n - i - 1));
                    }
                if (diff) continue;

                auto tl = self(self, cl), tr = self(self, cr);
                if (tl[0].size() != i || tr[0].size() != n - i - 1) continue;

                tl = array<string, 3>{v + tl[0] + tr[0], tl[1] + v + tr[1], tl[2] + tr[2] + v};
                t = !t[0].empty() && t < tl ? t : tl;
            }

            return cache[r] = t;
        };
        auto trees = construct(construct, routines);
        if (trees[0].empty()) continue;

        for (auto i : calls) cout << orders[i] << " ";
        cout << "\n";
        for (auto &t : trees) cout << t << "\n";
        cout << "\n";
    } while (next_permutation(calls.begin(), calls.end()));
}
