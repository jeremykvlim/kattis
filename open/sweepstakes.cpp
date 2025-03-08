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

void update(pair<vector<double>, int> &distribution, double p) {
    auto &[poly, offset] = distribution;
    poly.emplace_back(0);
    auto q = 1 - p;
    for (int i = poly.size() - 1; ~i; i--) {
        poly[i] *= q;
        if (i) poly[i] += poly[i - 1] * p;
    }

    int l = find_if(poly.begin(), poly.end(), [](auto value) {return value > 1e-14;}) - poly.begin(),
        r = find_if(poly.rbegin(), poly.rend(), [](auto value) {return value > 1e-14;}) - poly.rbegin();

    poly = {poly.begin() + l, poly.end() - r};
    offset += l;
}

struct SegmentTree {
    struct Segment {
        int queries;
        unordered_set<pair<int, int>, Hash> squares;
        Segment() : queries(0) {}

        auto & operator=(const vector<pair<int, int>> &v) {
            queries = v.size();
            for (auto [i, j] : v) squares.emplace(i, j);
            return *this;
        }

        friend auto operator+(const Segment &sl, const Segment &sr) {
            Segment seg;
            seg.queries = sl.queries + sr.queries;
            seg.squares.insert(sl.squares.begin(), sl.squares.end());
            seg.squares.insert(sr.squares.begin(), sr.squares.end());
            return seg;
        }
    };

    int n, t;
    vector<double> p, q;
    vector<Segment> ST;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void build() {
        for (int i = n - 1; i; i--) pull(i);
    }

    int midpoint(int l, int r) {
        int i = 1 << __lg(r - l);
        return min(l + i, r - (i >> 1));
    }

    void query(const pair<vector<double>, int> &distribution) {
        return query(distribution, 1, 0, n);
    }

    void query(const pair<vector<double>, int> &distribution1, int i, int l, int r) {
        if (l + 1 == r) {
            auto distribution2 = make_pair(vector<double>{1}, 0);
            for (auto [I, J] : ST[i].squares) update(distribution2, p[I] + q[J]);

            auto [poly1, offset1] = distribution1;
            auto [poly2, offset2] = distribution2;
            auto valid = [&](int j) {
                return 0 <= j - offset2 && j - offset2 < poly2.size() && 0 <= t - j - offset1 && t - j - offset1 < poly1.size();
            };

            double sum = 0;
            for (int j = 0; j <= ST[i].queries; j++)
                if (valid(j)) sum += poly2[j - offset2] * poly1[t - j - offset1];

            for (int j = 0; j <= ST[i].queries; j++) {
                if (valid(j)) cout << fixed << setprecision(6) << poly2[j - offset2] * poly1[t - j - offset1] / sum << " ";
                else cout << "0 ";
            }
            cout << "\n";
            return;
        }

        int m = midpoint(l, r);
        auto range_query = [&](int c, int l, int r) {
            auto temp = distribution1;
            for (auto [I, J] : ST[i].squares)
                if (!ST[c].squares.count({I, J})) update(temp, p[I] + q[J]);
            query(temp, c, l, r);
        };
        range_query(i << 1, l, m);
        range_query(i << 1 | 1, m, r);
    }

    auto & operator[](int i) {
        return ST[i];
    }

    SegmentTree(int n, const vector<vector<pair<int, int>>> &a, int t, const vector<double> &p, const vector<double> &q) : n(n), t(t), p(p), q(q), ST(2 * n) {
        int m = bit_ceil(a.size());
        for (int i = 0; i < a.size(); i++) ST[(i + m) % n + n] = a[i];
        build();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, n, t, Q;
    cin >> m >> n >> t >> Q;

    if (!Q) exit(0);

    vector<double> p(m + 1), q(n + 1);
    for (int i = 1; i <= m; i++) cin >> p[i];
    for (int i = 1; i <= n; i++) cin >> q[i];

    vector<vector<pair<int, int>>> queries(Q);
    for (auto &sq : queries) {
        int s;
        cin >> s;

        while (s--) {
            int i, j;
            cin >> i >> j;

            sq.emplace_back(i, j);
        }
    }
    SegmentTree st(Q, queries, t, p, q);

    auto distribution = make_pair(vector<double>{1}, 0);
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            if (!st[1].squares.count({i, j})) update(distribution, p[i] + q[j]);
    st.query(distribution);
}
