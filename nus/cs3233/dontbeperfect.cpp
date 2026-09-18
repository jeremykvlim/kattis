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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    if (n & 1) {
        cout << 1;
        exit(0);
    }

    vector<vector<int>> adj_list(n + 1);
    for (int i = 2; i <= n; i++) {
        int p;
        cin >> p;

        adj_list[p].emplace_back(i);
        adj_list[i].emplace_back(p);
    }

    auto dfs = [&](auto &&self, int v = 1, int prev = 0) -> vector<vector<int>> {
        vector<vector<int>> dp{{0}, {1}};
        for (int u : adj_list[v])
            if (u != prev) {
                unordered_set<vector<int>, Hash> temp;
                for (auto &cs1 : self(self, u, v))
                    for (auto &cs2 : dp) {
                        vector<int> component_sizes;
                        component_sizes.emplace_back(cs2[0] ? cs1[0] + cs2[0] : 0);
                        merge(cs2.begin() + 1, cs2.end(), cs1.begin() + 1, cs1.end(), back_inserter(component_sizes));
                        if (cs1[0] && !cs2[0]) component_sizes.insert(lower_bound(component_sizes.begin() + 1, component_sizes.end(), cs1[0]), cs1[0]);
                        temp.emplace(component_sizes);
                    }
                dp = {temp.begin(), temp.end()};
            }
        return dp;
    };

    unordered_set<vector<int>, Hash> good;
    for (auto component_sizes : dfs(dfs)) {
        if (!component_sizes[0]) component_sizes.erase(component_sizes.begin());
        else inplace_merge(component_sizes.begin(), component_sizes.begin() + 1, component_sizes.end());

        int m = n - accumulate(component_sizes.begin(), component_sizes.end(), 0);
        if (!m || component_sizes.size() < m + 2) continue;

        unordered_set<vector<int>, Hash> dp{{}};
        for (int size : component_sizes) {
            unordered_set<vector<int>, Hash> temp;
            for (auto &clique_sizes : dp) {
                if (clique_sizes.size() < m + 2) {
                    auto cs = clique_sizes;
                    cs.insert(lower_bound(cs.begin(), cs.end(), size), size);
                    temp.emplace(cs);
                }

                for (int i = 0; i < clique_sizes.size(); i++) {
                    if (i && clique_sizes[i] == clique_sizes[i - 1]) continue;
                    auto cs = clique_sizes;
                    cs[i] += size;
                    inplace_merge(cs.begin() + i, cs.begin() + i + 1, cs.end());
                    temp.emplace(cs);
                }
            }
            dp = temp;
        }

        for (auto &clique_sizes : dp)
            if (clique_sizes.size() == m + 2 && all_of(clique_sizes.begin(), clique_sizes.end(), [](int size) { return size & 1; })) good.emplace(clique_sizes);
    }
    cout << good.size();
}
