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

struct DisjointSets {
    vector<int> sets, size;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
            size[u_set] += size[v_set];
            size[v_set] = 0;
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n), size(n, 1) {
        iota(sets.begin(), sets.end(), 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    unordered_map<array<int, 4>, bool, Hash> memo;
    vector<bool> p1{true, true, false, false}, p2{false, true, false, true};
    vector<vector<int>> next(4, vector<int>(4));
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            next[i][j] = p2[i] ^ p2[j] ^ (!p1[i] || !p1[j]);
            if (!(p1[i] ^ p1[j])) next[i][j] += 2;
        }

    int t;
    cin >> t;

    while (t--) {
        int n, m;
        cin >> n >> m;

        DisjointSets dsu(n);
        vector<pair<int, int>> edges(m);
        for (auto &[u, v] : edges) {
            cin >> u >> v;
            u--;
            v--;

            dsu.unite(u, v);
        }

        vector<int> count(n, 0);
        for (auto [u, v] : edges) count[dsu.find(u)]++;

        int c0 = 0, c1 = 0, c2 = 0, c3 = 0;
        for (int i = 0; i < n; i++)
            if (dsu.size[i]) {
                int parity = ((dsu.size[i] >> 1) & 1) ^ (count[i] & 1);
                if (!(dsu.size[i] & 1)) parity += 2;
                if (!parity) c0++;
                else if (parity == 1) c1++;
                else if (parity == 2) c2++;
                else c3++;
            }

        auto dp = [&](auto &&self, int c0, int c1, int c2, int c3) -> bool {
            int comps = c0 + c1 + c2 + c3;
            if (comps <= 1) return false;

            array<int, 4> c{c0, c1, c2, c3};
            if (memo.count(c)) return memo[c];

            if ((c1 && !self(self, c0 + 1, c1 - 1, c2, c3)) || (c3 && !self(self, c0, c1, c2 + 1, c3 - 1))) return memo[c] = true;

            for (int i = 0; i < 4; i++)
                for (int j = i; j < 4; j++) {
                    if (!c[i]) break;
                    if ((i == j && c[i] < 2) || (i != j && !c[j])) continue;

                    int count0 = c0, count1 = c1, count2 = c2, count3 = c3;
                    if (!i) count0--;
                    else if (i == 1) count1--;
                    else if (i == 2) count2--;
                    else count3--;

                    if (!j) count0--;
                    else if (j == 1) count1--;
                    else if (j == 2) count2--;
                    else count3--;

                    int k = next[i][j];
                    if (!k) count0++;
                    else if (k == 1) count1++;
                    else if (k == 2) count2++;
                    else count3++;

                    if (!self(self, count0, count1, count2, count3)) return memo[c] = true;
                }
            return memo[c] = false;
        };
        cout << dp(dp, c0, c1, c2, c3) << "\n";
    }
}
