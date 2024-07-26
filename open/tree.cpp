#include <bits/stdc++.h>
using namespace std;

struct Hash {
    static uint64_t encode(array<string, 3> a) {
        auto encoded = 0ULL;
        for (auto e : a)
            for (auto c : e) encoded = (encoded << 8) | c;
        return encoded;
    }

    static uint64_t h(uint64_t key) {
        auto hash = key + 0x9e3779b97f4a7c15;
        hash = (hash ^ (hash >> 30)) * 0xbf58476d1ce4e5b9;
        hash = (hash ^ (hash >> 27)) * 0x94d049bb133111eb;
        hash = hash ^ (hash >> 31);
        return hash;
    }

    size_t operator()(array<string, 3> a) const {
        static uint64_t SEED = chrono::steady_clock::now().time_since_epoch().count();
        return h(encode(a) + SEED);
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

                array<string, 3> l, r;
                diff = (!r[1].empty() && r[1][i] != v) || (!r[2].empty() && r[2][n - 1] != v);
                for (int j = 2; ~j; j--)
                    if (!r[j].empty()) {
                        change(l[calls[j * 2]], r[j].substr(!j, i));
                        change(r[calls[j * 2 + 1]], r[j].substr(i + (j < 2), n - i - 1));
                    }
                if (diff) continue;

                auto tl = self(self, l), tr = self(self, r);
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
