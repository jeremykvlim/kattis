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
        if (lazy[i]) {
            apply(i << 1, lazy[i]);
            apply(i << 1 | 1, lazy[i]);
            lazy[i] = 0;
        }
    }

    void push_down(int l, int r) {
        for (int b = h; b; b--) {
            if (((l >> b) << b) != l) push(l >> b);
            if (((r >> b) << b) != r) push((r - 1) >> b);
        }
    }

    void pull_up(int l, int r) {
        for (int b = 1; b <= h; b++) {
            if (((l >> b) << b) != l) pull(l >> b);
            if (((r >> b) << b) != r) pull((r - 1) >> b);
        }
    }

    void range_update(int l, int r, const long long &v) {
        l += n;
        r += n;
        push_down(l, r);

        int temp_l = l, temp_r = r;
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) apply(l++, v);
            if (r & 1) apply(--r, v);
        }

        pull_up(temp_l, temp_r);
    }

    Monoid range_query(int l, int r) {
        l += n;
        r += n;
        push_down(l, r);

        Monoid ml, mr;
        for (; l < r; l >>= 1, r >>= 1) {
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
