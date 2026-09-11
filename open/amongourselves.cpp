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
struct AntiMonopolyTree {
    vector<int> parent, size;
    vector<T> weight;

    AntiMonopolyTree(int n) : parent(n, -1), size(n, 1), weight(n, numeric_limits<T>::max()) {}

    pair<T, int> path_max(int u, int v) {
        upward_maintain(u);
        upward_maintain(v);

        T max_w = numeric_limits<T>::min();
        int t = -1;
        while (u != v) {
            if (size[u] > size[v]) swap(u, v);
            if (weight[u] == numeric_limits<T>::max()) return {numeric_limits<T>::max(), -1};
            if (max_w < weight[u]) {
                max_w = weight[u];
                t = u;
            }
            u = parent[u];
        }
        return {max_w, t};
    }

    bool connected(int u, int v) {
        return u == v || path_max(u, v).second != -1;
    }

    void upward_maintain(int v) {
        while (~parent[v]) {
            int p = parent[v];
            if (3 * size[v] <= 2 * size[p]) {
                v = p;
                continue;
            }

            size[p] -= size[v];
            parent[v] = parent[p];
            if (weight[v] < weight[p]) {
                size[v] += size[p];
                swap(weight[v], weight[p]);
                parent[p] = v;
            }
        }
    }

    int root(int v) {
        while (~parent[v]) v = parent[v];
        return v;
    }

    void cut(int v) {
        for (int p = parent[v]; ~p; p = parent[p]) size[p] -= size[v];
        parent[v] = -1;
        weight[v] = numeric_limits<T>::max();
    }

    bool add(int u, int v, T w) {
        if (u == v) return false;

        upward_maintain(u);
        upward_maintain(v);

        auto [max_w, t] = path_max(u, v);
        bool merged = max_w == numeric_limits<T>::max();
        if (!merged) {
            if (w >= max_w) return false;
            cut(t);
        }

        int du = 0, dv = 0;
        while (~u && ~v) {
            if (w >= weight[u]) {
                int p = parent[u];
                if (~p) size[p] += du;
                u = p;
            } else if (w >= weight[v]) {
                int p = parent[v];
                if (~p) size[p] += dv;
                v = p;
            } else {
                if (size[u] > size[v]) {
                    swap(u, v);
                    swap(du, dv);
                }

                du -= size[u];
                dv += size[u];
                size[v] += size[u];
                w = exchange(weight[u], w);
                u = exchange(parent[u], v);
                if (~u) size[u] += du;
            }
        }

        if (~v)
            for (v = parent[v]; ~v; v = parent[v]) size[v] += dv;
        return merged;
    }

    bool remove(int u, int v, T w) {
        auto [max_w, t] = path_max(u, v);
        if (max_w != w) return false;

        cut(t);
        return true;
    }
};

struct OfflineDynamicGraph {
    AntiMonopolyTree<int> amt;
    vector<array<int, 3>> events;
    vector<pair<int, function<void(AntiMonopolyTree<int> &)>>> queries;
    unordered_map<pair<int, int>, pair<int, int>, Hash> active;

    OfflineDynamicGraph(int n) : amt(n) {}

    void add_edge(int u, int v) {
        if (u > v) swap(u, v);

        auto [it, inserted] = active.try_emplace({u, v}, events.size(), 1);
        if (!inserted) {
            it->second.second++;
            return;
        }
        events.push_back({u, v, 0});
    }

    void delete_edge(int u, int v) {
        if (u > v) swap(u, v);

        auto it = active.find({u, v});
        if (--it->second.second) return;
        events.push_back({u, v, 1});
        events[it->second.first][2] = 1 - events.size();
        active.erase(it);
    }

    template <typename F>
    void query(F &&f) {
        queries.emplace_back(events.size(), f);
    }

    void process() {
        int q = 0;
        for (int i = 0; i < events.size(); i++) {
            for (; q < queries.size() && queries[q].first == i; q++) queries[q].second(amt);

            auto [u, v, w] = events[i];
            if (w == 1) amt.remove(u, v, -i);
            else amt.add(u, v, w ? w : -events.size() - 1);
        }
        for (; q < queries.size(); q++) queries[q].second(amt);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k, t;
    cin >> n >> m >> k >> t;

    vector<array<int, 3>> edges(m);
    for (auto &[a, b, s] : edges) {
        string S;
        cin >> a >> b >> S;
        a--;
        b--;

        s = S == "imposter";
    }

    OfflineDynamicGraph odg(2 * n);
    auto link = [&](int i) {
        auto [u, v, x] = edges[i];
        odg.add_edge(u, v + x * n);
        odg.add_edge(u + n, v + (x ^ 1) * n);
    };

    auto cut = [&](int i) {
        auto [u, v, x] = edges[i];
        odg.delete_edge(u, v + x * n);
        odg.delete_edge(u + n, v + (x ^ 1) * n);
    };
    for (int i = t; i < m; i++) link(i);

    int j = -1;
    bool imposter = false;
    for (int i = 0; i <= m - t; i++) {
        odg.query([&, i](auto &amt) {
            if (~j) return;

            if (amt.connected(0, k - 1)) {
                j = i + 1;
                imposter = false;
            } else if (amt.connected(0, k - 1 + n)) {
                j = i + 1;
                imposter = true;
            }
        });
        if (i == m - t) break;

        link(i);
        cut(i + t);
    }
    odg.process();

    if (!~j) cout << -1;
    else cout << j << (imposter ? " imposter" : " crewmate");
}