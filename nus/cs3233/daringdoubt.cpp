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

    void range_update(int l, int r, const int &v) {
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

    int d, c, m;
    cin >> d >> c >> m;

    vector<int> indices_d(m + 1, -1);
    for (int i = 1; i <= d; i++) {
        int a;
        cin >> a;

        indices_d[a] = i;
    }

    auto count = 0LL;
    stack<int> mono_inc, mono_dec;
    mono_inc.emplace(0);
    mono_dec.emplace(0);
    vector<int> indices_both(c + 1, -1);
    RURQSegmentTree st(bit_ceil((unsigned) c + 1), vector<int>(c + 1, 0));
    for (int i = 1, l = 0; i <= c; i++) {
        int b;
        cin >> b;

        indices_both[i] = indices_d[b];
        if (indices_both[i] == -1) l = i;

        st.range_update(i, i + 1, i);

        while (mono_inc.size() > 1 && indices_both[mono_inc.top()] >= indices_both[i]) {
            int v = mono_inc.top();
            mono_inc.pop();

            int u = mono_inc.top();
            st.range_update(u + 1, v + 1, indices_both[v] - indices_both[i]);
        }
        mono_inc.emplace(i);

        while (mono_dec.size() > 1 && indices_both[mono_dec.top()] <= indices_both[i]) {
            int v = mono_dec.top();
            mono_dec.pop();

            int u = mono_dec.top();
            st.range_update(u + 1, v + 1, indices_both[i] - indices_both[v]);
        }
        mono_dec.emplace(i);

        auto [v, f] = st.range_query(l + 1, i + 1);
        if (v == i) count += f;
    }
    cout << count;
}