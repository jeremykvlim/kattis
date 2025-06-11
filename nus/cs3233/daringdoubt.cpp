#include <bits/stdc++.h>
using namespace std;

struct SegmentTree {
    struct Segment {
        int value, freq;

        Segment() : value(INT_MAX), freq(0) {}

        auto & operator=(const int &v) {
            value = v;
            freq = 1;
            return *this;
        }

        auto operator+=(const int &v) {
            value += v;
            return *this;
        }

        auto operator+=(const Segment &seg) {
            if (value < seg.value) return *this;
            else if (value > seg.value) return seg;
            else freq += seg.freq;

            return *this;
        }

        friend auto operator+(Segment sl, const Segment &sr) {
            return sl += sr;
        }
    };

    int n, h;
    vector<Segment> ST;
    vector<int> lazy;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void apply(int i, const int &v) {
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

    void assign(int i, const int &v) {
        for (ST[i += n] = v; i > 1; i >>= 1) pull(i >> 1);
    }

    void modify(int l, int r, const int &v) {
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

    Segment range_query(int l, int r) {
        push(l + n);
        push(r + n - 1);
        Segment seg;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) seg = seg + ST[l++];
            if (r & 1) seg = ST[--r] + seg;
        }

        return seg;
    }

    auto & operator[](int i) {
        return ST[i];
    }

    SegmentTree(int n, const vector<int> &a) : n(n), h(__lg(n)), ST(2 * n), lazy(n, 0) {
        for (int i = 0; i < a.size(); i++) assign(i, a[i]);
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
    SegmentTree st(c + 1, vector<int>(c + 1, 0));
    for (int i = 1, l = 0; i <= c; i++) {
        int b;
        cin >> b;

        indices_both[i] = indices_d[b];
        if (indices_both[i] == -1) l = i;

        st.modify(i, i + 1, i);

        while (mono_inc.size() > 1 && indices_both[mono_inc.top()] >= indices_both[i]) {
            int v = mono_inc.top();
            mono_inc.pop();

            int u = mono_inc.top();
            st.modify(u + 1, v + 1, indices_both[v] - indices_both[i]);
        }
        mono_inc.emplace(i);

        while (mono_dec.size() > 1 && indices_both[mono_dec.top()] <= indices_both[i]) {
            int v = mono_dec.top();
            mono_dec.pop();

            int u = mono_dec.top();
            st.modify(u + 1, v + 1, indices_both[i] - indices_both[v]);
        }
        mono_dec.emplace(i);

        auto [v, f] = st.range_query(l + 1, i + 1);
        if (v == i) count += f;
    }
    cout << count;
}
