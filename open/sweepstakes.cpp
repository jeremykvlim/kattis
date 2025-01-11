#include <bits/stdc++.h>
using namespace std;

struct Hash {
    size_t operator()(pair<int, int> p) const {
        auto h = hash<int>()(p.first);
        h ^= hash<int>()(p.second) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
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
            Segment sm;
            sm.queries = sl.queries + sr.queries;
            for (auto [i, j] : sl.squares) sm.squares.emplace(i, j);
            for (auto [i, j] : sr.squares) sm.squares.emplace(i, j);
            return sm;
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
        int i = 1 << __lg(r - l + 1);
        return min(l + i - 1, r - (i >> 1));
    }

    void query(const pair<vector<double>, int> &distribution1, int k, int l, int r) {
        if (l == r) {
            auto distribution2 = make_pair(vector<double>{1}, 0);
            for (auto [i, j] : ST[k].squares) update(distribution2, p[i] + q[j]);

            auto [poly1, offset1] = distribution1;
            auto [poly2, offset2] = distribution2;
            auto valid = [&](int i) {
                return 0 <= i - offset2 && i - offset2 < poly2.size() && 0 <= t - i - offset1 && t - i - offset1 < poly1.size();
            };

            double sum = 0;
            for (int i = 0; i <= ST[k].queries; i++)
                if (valid(i)) sum += poly2[i - offset2] * poly1[t - i - offset1];

            for (int i = 0; i <= ST[k].queries; i++) {
                if (valid(i)) cout << fixed << setprecision(7) << poly2[i - offset2] * poly1[t - i - offset1] / sum << " ";
                else cout << "0 ";
            }
            cout << "\n";
            return;
        }

        int m = midpoint(l, r);
        auto range_query = [&](int c, int l, int r) {
            auto temp = distribution1;
            for (auto [i, j] : ST[k].squares)
                if (!ST[c].squares.count({i, j})) update(temp, p[i] + q[j]);
            query(temp, c, l, r);
        };
        range_query(k << 1, l, m);
        range_query(k << 1 | 1, m + 1, r);
    }

    void query(const pair<vector<double>, int> &distribution) {
        return query(distribution, 1, 1, n);
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
