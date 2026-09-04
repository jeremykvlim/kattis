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

struct AntiMonopolyTree {
    vector<int> parent, size, weight;
    long long sum;

    AntiMonopolyTree(int n) : parent(n, -1), size(n, 1), weight(n, INT_MAX), sum(0) {}

    pair<int, int> path_max(int u, int v) {
        upward_maintain(u);
        upward_maintain(v);

        int max_w = INT_MIN, t = -1;
        while (u != v) {
            if (size[u] > size[v]) swap(u, v);
            if (weight[u] == INT_MAX) return {INT_MAX, -1};
            if (max_w < weight[u]) {
                max_w = weight[u];
                t = u;
            }
            u = parent[u];
        }
        return {max_w, t};
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
        weight[v] = INT_MAX;
    }

    bool add(int u, int v, int w) {
        if (u == v) return false;

        upward_maintain(u);
        upward_maintain(v);

        auto [max_w, t] = path_max(u, v);
        bool merged = max_w == INT_MAX;
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

    bool remove(int u, int v, int w) {
        auto [max_w, t] = path_max(u, v);
        if (max_w != w) return false;

        sum -= (long long) size[t] * (size[root(t)] - size[t]);
        cut(t);
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<pair<int, int>> path(m);
    for (auto &e : path) {
        int a, b;
        cin >> a >> b;

        e = minmax(a - 1, b - 1);
    }

    int q;
    cin >> q;

    vector<array<int, 3>> edges(m);
    unordered_map<pair<int, int>, int, Hash> active;
    for (int i = 0; i < m; i++) {
        auto [u, v] = path[i];
        edges[i] = {u, v, q};
        active[path[i]] = i;
    }

    vector<int> t(q), add(q, -1), remove(q, -1);
    for (int i = 0; i < q; i++) {
        cin >> t[i];

        if (t[i] == 1 || t[i] == 2) {
            int a, b;
            cin >> a >> b;

            pair<int, int> e = minmax(a - 1, b - 1);
            if (t[i] == 1) {
                if (!active.count(e)) {
                    edges.push_back({a - 1, b - 1, q});
                    add[i] = active[e] = edges.size() - 1;
                }
            } else if (active.count(e)) {
                edges[remove[i] = active[e]][2] = i;
                active.erase(e);
            }
        }
    }

    int k = edges.size();
    vector<int> order(k);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j) { return edges[i][2] != edges[j][2] ? edges[i][2] > edges[j][2] : i < j; });
    for (int i = 0; i < k; i++) edges[order[i]][2] = i;

    AntiMonopolyTree amt(n);
    for (int e = 0; e < m; e++) {
        auto [u, v, w] = edges[e];
        amt.add(u, v, w);
    }

    auto total = (long long) n * (n - 1) / 2;
    for (int i = 0; i < q; i++)
        if (t[i] == 1) {
            if (add[i] != -1) {
                int e = add[i];
                auto [u, v, w] = edges[e];
                amt.add(u, v, w);
                m++;
            }
        } else if (t[i] == 2) {
            if (remove[i] != -1) {
                int e = remove[i];
                auto [u, v, w] = edges[e];
                amt.remove(u, v, w);
                m--;
            }
        } else {
            Fraction<long long> f(total - amt.sum, total - m);
            if (!f.numer()) cout << "0/1\n";
            else cout << f.numer() << "/" << f.denom() << "\n";
        }
}