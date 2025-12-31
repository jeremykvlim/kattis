#include <bits/stdc++.h>
using namespace std;

struct RURQSegmentTree {
    struct Segment {
        int value, freq;

        Segment() : value(INT_MAX), freq(0) {}

        auto & operator=(const int &v) {
            value = v;
            freq = 1;
            return *this;
        }

        auto & operator+=(const int &v) {
            value += v;
            return *this;
        }

        auto & operator+=(const Segment &seg) {
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

    void build() {
        for (int i = n - 1; i; i--) pull(i);
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

    Segment range_query(int l, int r) {
        push(l + n);
        push(r + n - 1);
        Segment sl, sr;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) sl = sl + ST[l++];
            if (r & 1) sr = ST[--r] + sr;
        }

        return sl + sr;
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

    RURQSegmentTree st(n, count);
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