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
struct Fraction : array<T, 2> {
    using F = array<T, 2>;

    Fraction() = default;
    Fraction(T n, T d) : F{n, d} {
        reduce();
    }

    T & numer() {
        return (*this)[0];
    }

    T & denom() {
        return (*this)[1];
    }

    const T & numer() const {
        return (*this)[0];
    }

    const T & denom() const {
        return (*this)[1];
    }

    void reduce() {
        if (denom() < 0) {
            numer() *= -1;
            denom() *= -1;
        }

        T g = __gcd(abs(numer()), denom());
        if (g) {
            numer() /= g;
            denom() /= g;
        }
    }

    bool operator<(const Fraction &f) const {
        return numer() * f.denom() < f.numer() * denom();
    }

    bool operator>(const Fraction &f) const {
        return numer() * f.denom() > f.numer() * denom();
    }

    bool operator==(const Fraction &f) const {
        return numer() == f.numer() && denom() == f.denom();
    }

    bool operator!=(const Fraction &f) const {
        return numer() != f.numer() || denom() != f.denom();
    }

    bool operator<=(const Fraction &f) const {
        return *this < f || *this == f;
    }

    bool operator>=(const Fraction &f) const {
        return *this > f || *this == f;
    }

    Fraction operator+(const Fraction &f) const {
        return {numer() * f.denom() + f.numer() * denom(), denom() * f.denom()};
    }

    Fraction operator+(const T &v) const {
        return {numer() + v * denom(), denom()};
    }

    Fraction & operator+=(const Fraction &f) {
        numer() = numer() * f.denom() + f.numer() * denom();
        denom() *= f.denom();
        reduce();
        return *this;
    }

    Fraction & operator+=(const T &v) {
        numer() += v * denom();
        reduce();
        return *this;
    }

    Fraction operator-(const Fraction &f) const {
        return {numer() * f.denom() - f.numer() * denom(), denom() * f.denom()};
    }

    Fraction operator-(const T &v) const {
        return {numer() - v * denom(), denom()};
    }

    Fraction & operator-=(const Fraction &f) {
        numer() = numer() * f.denom() - f.numer() * denom();
        denom() *= f.denom();
        reduce();
        return *this;
    }

    Fraction & operator-=(const T &v) {
        numer() -= v * denom();
        reduce();
        return *this;
    }

    Fraction operator*(const Fraction &f) const {
        return {numer() * f.numer(), denom() * f.denom()};
    }

    Fraction operator*(const T &v) const {
        return {numer() * v, denom()};
    }

    Fraction & operator*=(const Fraction &f) {
        numer() *= f.numer();
        denom() *= f.denom();
        reduce();
        return *this;
    }

    Fraction & operator*=(const T &v) {
        numer() *= v;
        reduce();
        return *this;
    }

    Fraction operator/(const Fraction &f) const {
        return {numer() * f.denom(), denom() * f.numer()};
    }

    Fraction operator/(const T &v) const {
        return {numer(), denom() * v};
    }

    Fraction & operator/=(const Fraction &f) {
        numer() *= f.denom();
        denom() *= f.numer();
        reduce();
        return *this;
    }

    Fraction & operator/=(const T &v) {
        denom() *= v;
        reduce();
        return *this;
    }
};

template <typename T>
struct AntiMonopolyTree {
    vector<int> parent, size;
    vector<T> weight;
    long long sum;

    AntiMonopolyTree(int n) : parent(n, -1), size(n, 1), weight(n, numeric_limits<T>::max()), sum(0) {}

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

        auto [max_w, t] = path_max(u, v);
        bool merged = max_w == numeric_limits<T>::max();
        if (!merged) {
            if (w >= max_w) return false;
            cut(t);
        } else sum += (long long) size[root(u)] * size[root(v)];

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

        sum -= (long long) size[t] * (size[root(t)] - size[t]);
        cut(t);
        return true;
    }
};

struct OfflineDynamicGraph {
    AntiMonopolyTree<int> amt;
    vector<array<int, 3>> events;
    vector<pair<int, function<void(AntiMonopolyTree<int> &)>>> queries;
    unordered_map<pair<int, int>, int, Hash> active;

    OfflineDynamicGraph(int n) : amt(n) {}

    bool add_edge(int u, int v) {
        if (u > v) swap(u, v);

        auto [it, inserted] = active.try_emplace({u, v}, events.size());
        if (!inserted) return false;
        events.push_back({u, v, 0});
        return true;
    }

    bool delete_edge(int u, int v) {
        if (u > v) swap(u, v);

        auto it = active.find({u, v});
        if (it == active.end()) return false;
        events.push_back({u, v, 1});
        events[it->second][2] = 1 - events.size();
        active.erase(it);
        return true;
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

    int n, m;
    cin >> n >> m;

    OfflineDynamicGraph odg(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;

        odg.add_edge(u - 1, v - 1);
    }

    int q;
    cin >> q;

    auto total = (long long) n * (n - 1) / 2;
    while (q--) {
        int t;
        cin >> t;

        if (t == 1 || t == 2) {
            int u, v;
            cin >> u >> v;
            u--;
            v--;

            if (t == 1) m += odg.add_edge(u, v);
            else m -= odg.delete_edge(u, v);
        } else {
            odg.query([total, m](auto &amt) {
                Fraction<long long> f(total - amt.sum, total - m);
                if (!f.numer()) cout << "0/1\n";
                else cout << f.numer() << "/" << f.denom() << "\n";
            });
        }
    }
    odg.process();
}