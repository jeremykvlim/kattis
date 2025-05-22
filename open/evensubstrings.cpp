#include <bits/stdc++.h>
using namespace std;

struct SegmentTree {
    static inline array<array<int, 64>, 64> sgn;
    struct Segment {
        vector<long long> F;

        Segment() : F(64, 0) {}

        auto & operator=(const int &v) {
            for (int i = 0; i < 64; i++) F[i] = sgn[v][i];
            return *this;
        }

        auto operator+=(const int &v) {
            for (int i = 0; i < 64; i++) F[i] *= sgn[v][i];
            return *this;
        }

        auto operator+=(const Segment &seg) {
            for (int i = 0; i < 64; i++) F[i] += seg.F[i];
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
        if (i < n) lazy[i] ^= v;
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

    auto range_query(int l, int r) {
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
        for (int mask = 0; mask < 64; mask++)
            for (int i = 0; i < 64; i++) sgn[mask][i] = (popcount((unsigned) mask & i) & 1) ? -1 : 1;

        for (int i = 0; i < a.size(); i++) assign(i, a[i]);
    }
};

template <typename T>
void fwht(int n, vector<T> &v, bool inv = false) {
    for (int k = 1; k < n; k <<= 1)
        for (int i = 0; i < n; i += k << 1)
            for (int j = 0; j < k; j++) {
                auto t = v[i + j + k];
                v[i + j + k] = v[i + j] - t;
                v[i + j] += t;
            }

    if (inv)
        for (T &e : v) e /= n;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    int q;
    cin >> s >> q;

    int n = s.size();
    s = "#" + s;

    vector<int> pref(n + 1, 0);
    for (int i = 1; i <= n; i++) pref[i] = pref[i - 1] ^ (1 << (s[i] - 'a'));

    SegmentTree st(bit_ceil((unsigned) n + 1), pref);
    while (q--) {
        int t;
        cin >> t;

        if (t == 1) {
            int l, r;
            cin >> l >> r;

            auto f = st.range_query(l - 1, r + 1).F;
            fwht(64, f, true);
            auto count = 0LL;
            for (auto &c : f) count += c * (c - 1) / 2;
            cout << count << "\n";
        } else {
            int i;
            char x;
            cin >> i >> x;

            if (s[i] == x) continue;

            char c = s[i];
            s[i] = x;
            st.modify(i, n + 1, (1 << (c - 'a')) ^ (1 << (x - 'a')));
        }
    }
}
