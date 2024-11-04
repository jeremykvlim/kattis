#include <bits/stdc++.h>
using namespace std;

struct Hash {
    static uint64_t encode(pair<int, int> p) {
        auto encoded = 0ULL;
        encoded = (encoded << 8) | p.first;
        encoded = (encoded << 8) | p.second;
        return encoded;
    }

    static uint64_t h(uint64_t key) {
        auto hash = key + 0x9e3779b97f4a7c15;
        hash = (hash ^ (hash >> 30)) * 0xbf58476d1ce4e5b9;
        hash = (hash ^ (hash >> 27)) * 0x94d049bb133111eb;
        hash = hash ^ (hash >> 31);
        return hash;
    }

    size_t operator()(pair<int, int> p) const {
        static uint64_t SEED = chrono::steady_clock::now().time_since_epoch().count();
        return h(encode(p) + SEED);
    }
};

struct DisjointSet {
    vector<int> sets;

    int find(int p) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p]));
    }

    bool unite(int p, int q) {
        int p_set = find(p), q_set = find(q);
        if (p_set != q_set) {
            sets[q_set] = p_set;
            return true;
        }
        return false;
    }

    DisjointSet(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> adj_list(n);
    for (int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;

        adj_list[a].emplace_back(b);
        adj_list[b].emplace_back(a);
    }

    unordered_set<pair<int, int>, Hash> plan;
    for (int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;

        if (a > b) swap(a, b);
        plan.emplace(a, b);
    }

    vector<int> depth(n), prev(n);
    auto dfs = [&](auto &&self, int v = 0, int p = -1) -> void {
        depth[v] = depth[p] + 1;
        prev[v] = p;
        for (int u : adj_list[v])
            if (u != p) self(self, u, v);
    };
    dfs(dfs);

    DisjointSet dsu(n);
    for (int a = 0; a < n; a++)
        for (int b : adj_list[a]) {
            if (a > b) continue;

            if (plan.count({a, b})) {
                plan.erase({a, b});

                if (prev[a] == b) dsu.unite(b, a);
                else dsu.unite(a, b);
            }
        }

    cout << plan.size() << "\n";
    for (auto [a2, b2] : plan) {
        int a1 = dsu.find(a2), b1 = dsu.find(b2);
        if (depth[a1] < depth[b1]) {
            swap(a1, b1);
            swap(a2, b2);
        }
        b1 = prev[a1];

        cout << a1 << " " << b1 << " " << a2 << " " << b2 << "\n";
        dsu.unite(b2, a2);
    }
}
