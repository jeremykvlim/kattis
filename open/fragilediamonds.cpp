#include <bits/stdc++.h>
using namespace std;

struct SegmentTree {
    struct Monoid {
        long long minimum, maximum;

        Monoid() : minimum(1e18), maximum(-1e18) {}

        auto & operator+=(const Monoid &monoid) {
            minimum = min(minimum, monoid.minimum);
            maximum = max(maximum, monoid.maximum);
            return *this;
        }

        friend auto operator+(Monoid ml, const Monoid &mr) {
            ml += mr;
            return ml;
        }
    };

    int n;
    vector<Monoid> ST;
    vector<int> indices;

    SegmentTree(int n) : n(n), ST(2 * n), indices(n, -1) {}

    int midpoint(int l, int r) {
        int i = 1 << __lg(r - l);
        return min(l + i, r - (i >> 1));
    }

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void point_update(int p, long long minimum, long long maximum, int k) {
        point_update(1, p, minimum, maximum, k, 0, n);
    }

    void point_update(int i, int pos, long long minimum, long long maximum, int k, int l, int r) {
        if (l + 1 == r) {
            ST[i].minimum = minimum;
            ST[i].maximum = maximum;
            indices[l] = k;
            return;
        }

        int m = midpoint(l, r);
        if (pos < m) point_update(i << 1, pos, minimum, maximum, k, l, m);
        else point_update(i << 1 | 1, pos, minimum, maximum, k, m, r);

        pull(i);
    }

    vector<int> range_update(int ql, int qr, long long x, bool b) {
        vector<int> broken;
        range_update(1, ql, qr, x, b, broken, 0, n);
        return broken;
    }

    void range_update(int i, int ql, int qr, long long x, int b, vector<int> &broken, int l, int r) {
        if (qr <= l || r <= ql || (!b ? ST[i].maximum <= x : ST[i].minimum >= x)) return;
        if (l + 1 == r) {
            ST[i] = {};
            broken.emplace_back(indices[l]);
            return;
        }

        int m = midpoint(l, r);
        range_update(i << 1, ql, qr, x, b, broken, l, m);
        range_update(i << 1 | 1, ql, qr, x, b, broken, m, r);

        pull(i);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<long long> x(n), y(n), xs;
    for (int i = 0; i < n; i++) {
        cin >> x[i] >> y[i];

        xs.emplace_back(x[i]);
    }
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());

    SegmentTree st(xs.size());
    for (int i = 0; i < n; i++) {
        auto l = x[i] - 2 * y[i], r = x[i] + 2 * y[i];
        int p = lower_bound(xs.begin(), xs.end(), x[i]) - xs.begin();

        auto broken_l = st.range_update(upper_bound(xs.begin(), xs.end(), l) - xs.begin(), p + 1, x[i], false);
        auto broken_r = st.range_update(p + 1, lower_bound(xs.begin(), xs.end(), r) - xs.begin(), x[i], true);
        broken_l.insert(broken_l.end(), broken_r.begin(), broken_r.end());
        sort(broken_l.begin(), broken_l.end());
        cout << broken_l.size() << " ";
        for (int j : broken_l) cout << j << " ";
        cout << "\n";

        st.point_update(p, l, r, i + 1);
    }
}
