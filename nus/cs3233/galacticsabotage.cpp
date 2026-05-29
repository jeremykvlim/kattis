#include <bits/stdc++.h>
using namespace std;

struct RURQSegmentTree {
    struct Monoid {
        int value, freq;

        Monoid() : value(INT_MAX), freq(0) {}

        auto & operator=(const int &v) {
            value = v;
            freq = 1;
            return *this;
        }

        auto & operator+=(const int &v) {
            value += v;
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            if (value < monoid.value) return *this;
            if (value > monoid.value) return *this = monoid;
            freq += monoid.freq;
            return *this;
        }

        friend auto operator+(Monoid ml, const Monoid &mr) {
            return ml += mr;
        }
    };

    int n, h;
    vector<Monoid> ST;
    vector<int> lazy;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void build() {
        for (int i = n - 1; i; i--) pull(i);
    }

    void apply(int i, const int &v) {
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

    RURQSegmentTree(int n, const vector<int> &a) : n(n), h(__lg(n)), ST(2 * n), lazy(n, 0) {
        for (int i = 0; i < a.size(); i++) ST[i + n] = a[i];
        build();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> a(n), count(n);
    vector<bool> seen(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        a[i]--;

        if (i) count[i] = count[i - 1];
        if (seen[i]) {
            seen[i] = false;
            count[i]--;
        }

        if (a[i] > i) {
            seen[a[i]] = true;
            count[i]++;
        }
    }

    RURQSegmentTree st(bit_ceil((unsigned) n), count);
    cout << st[1].freq << "\n";
    while (m--) {
        int u, v;
        cin >> u >> v;
        u--;
        v--;

        auto process = [&](int l, int r, int v) {
            if (l < r) st.range_update(l, r, v);
        };

        process(u, a[v], 1);
        process(v, a[u], 1);
        process(u, a[u], -1);
        process(v, a[v], -1);
        swap(a[u], a[v]);

        cout << st[1].freq << "\n";
    }
}