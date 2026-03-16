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

struct WeightedDisjointSets {
    vector<int> sets, prio, size;
    vector<pair<int, int>> weight;
    long long sum;

    WeightedDisjointSets(int n) : sets(n), prio(n), size(n, 1), weight(n, {INT_MAX, 0}), sum(0) {
        iota(sets.begin(), sets.end(), 0);
        iota(prio.begin(), prio.end(), 0);
        shuffle(prio.begin(), prio.end(), mt19937_64(random_device{}()));
    }

    int & compress(int v) {
        if (sets[v] == v) return sets[v];
        while (weight[sets[v]].first <= weight[v].first) {
            size[sets[v]] -= size[v];
            sets[v] = sets[sets[v]];
        }
        return sets[v];
    }

    int find(int v, int w = INT_MAX - 1) {
        while (weight[v].first <= w) v = compress(v);
        return v;
    }

    void detach(int v) {
        if (sets[v] == v) return;
        detach(sets[v]);
        size[sets[v]] -= size[v];
    }

    int attach(int v, int w = INT_MAX - 1) {
        while (weight[v].first <= w) {
            size[sets[v]] += size[v];
            v = sets[v];
        }
        return v;
    }

    void link(int u, int v, pair<int, int> w) {
        sum += (long long) component_size(u) * component_size(v);
        detach(u);
        detach(v);
        while (u != v) {
            u = attach(u, w.first);
            v = attach(v, w.first);
            if (prio[u] < prio[v]) swap(u, v);
            swap(sets[v], u);
            swap(weight[v], w);
        }
        attach(u);
    }

    void cut(int v, int w) {
        while (sets[v] != v) {
            if (weight[v].first == w) {
                int u = v;
                for (; u != sets[u]; u = sets[u]);
                sum -= (long long) size[v] * (size[u] - size[v]);

                for (u = v; u != sets[u]; size[u = sets[u]] -= size[v]);
                sets[v] = v;
                weight[v] = {INT_MAX, 0};
                return;
            }
            v = compress(v);
        }
    }

    void cut(int u, int v, int w) {
        cut(u, w);
        cut(v, w);
    }

    int path_max(int u, int v) {
        if (find(u) != find(v)) return -1;

        for (;;) {
            if (weight[u].first > weight[v].first) swap(u, v);
            if (sets[u] == v) return u;
            u = sets[u];
        }
    }

    int unite(int u, int v, pair<int, int> w) {
        if (u != v) {
            int t = path_max(u, v);
            if (t == -1) {
                link(u, v, w);
                return -1;
            } else if (weight[t].first > w.first) {
                int i = weight[t].second;
                cut(t, weight[t].first);
                link(u, v, w);
                return i;
            }
        }
        return w.second;
    }

    int component_size(int v) {
        return size[find(v)];
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

    vector<int> order(edges.size());
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j) { return edges[i][2] != edges[j][2] ? edges[i][2] > edges[j][2] : i < j; });
    for (int i = 0; i < order.size(); i++) edges[order[i]][2] = i;

    WeightedDisjointSets wdsu(n);
    for (int e = 0; e < m; e++) {
        auto [u, v, w] = edges[e];
        wdsu.unite(u, v, {w, e});
    }

    auto total = (long long) n * (n - 1) / 2;
    for (int i = 0; i < q; i++)
        if (t[i] == 1) {
            if (add[i] != -1) {
                int e = add[i];
                auto [u, v, w] = edges[e];
                wdsu.unite(u, v, {w, e});
                m++;
            }
        } else if (t[i] == 2) {
            if (remove[i] != -1) {
                int e = remove[i];
                auto [u, v, w] = edges[e];
                wdsu.cut(u, v, w);
                m--;
            }
        } else {
            Fraction<long long> f(total - wdsu.sum, total - m);
            if (!f.numer()) cout << "0/1\n";
            else cout << f.numer() << "/" << f.denom() << "\n";
        }
}
