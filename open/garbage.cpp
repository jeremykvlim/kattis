#include <bits/stdc++.h>
using namespace std;

struct RURQSegmentTree {
    struct Monoid {
        long long value;

        Monoid() : value(0) {}

        auto & operator+=(const long long &v) {
            value += v;
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            value = max(value, monoid.value);
            return *this;
        }

        friend auto operator+(Monoid ml, const Monoid &mr) {
            return ml += mr;
        }
    };

    int n, h;
    vector<Monoid> ST;
    vector<long long> lazy;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void apply(int i, const long long &v) {
        ST[i] += v;
        if (i < n) lazy[i] += v;
    }

    void push(int i) {
        for (int k = h; k; k--) {
            int j = i >> k;
            if (lazy[j]) {
                apply(j << 1, lazy[j]);
                apply(j << 1 | 1, lazy[j]);
                lazy[j] = 0;
            }
        }
    }

    void range_update(int l, int r, const long long &v) {
        push(l + n);
        push(r + n - 1);
        bool cl = false, cr = false;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (cl) pull(l - 1);
            if (cr) pull(r);
            if (l & 1) {
                cl = true;
                apply(l++, v);
            }
            if (r & 1) {
                cr = true;
                apply(--r, v);
            }
        }

        for (l--; r; l >>= 1, r >>= 1) {
            if (cl) pull(l);
            if (cr && (!cl || l < r)) pull(r);
        }
    }

    Monoid range_query(int l, int r) {
        push(l + n);
        push(r + n - 1);
        Monoid ml, mr;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) ml = ml + ST[l++];
            if (r & 1) mr = ST[--r] + mr;
        }

        return ml + mr;
    }

    auto & operator[](int i) {
        return ST[i];
    }

    RURQSegmentTree(int n) : n(n), h(__lg(n)), ST(2 * n), lazy(n, 0) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, W, h;
    cin >> n >> W >> h;

    vector<array<int, 3>> garbage(n);
    vector<int> xs(n), ys(n);
    for (int i = 0; i < n; i++) {
        auto &[x, y, w] = garbage[i];
        cin >> x >> y >> w;

        xs[i] = x;
        ys[i] = y;
    }
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());

    sort(garbage.begin(), garbage.end());
    RURQSegmentTree st(bit_ceil(ys.size()));
    auto update = [&](int i, int w) {
        int l = upper_bound(ys.begin(), ys.end(), garbage[i][1] - h) - ys.begin(), r = upper_bound(ys.begin(), ys.end(), garbage[i][1]) - ys.begin();
        if (l < r) st.range_update(l, r, w);
    };

    auto weight = 0LL;
    for (int add = 0, remove = 0; int x : xs) {
        for (; add < n && garbage[add][0] < x + W; add++) update(add, garbage[add][2]);
        for (; remove < n && garbage[remove][0] < x; remove++) update(remove, -garbage[remove][2]);
        weight = max(weight, st[1].value);
    }
    cout << weight;
}
