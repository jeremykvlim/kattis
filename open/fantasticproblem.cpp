#include <bits/stdc++.h>
using namespace std;

struct SegmentTree {
    struct Segment {
        long long value, freq;

        Segment() : value(0), freq(0) {}

        auto & operator=(const long long &v) {
            freq = v;
            return *this;
        }

        auto operator+=(const long long &v) {
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

    void assign(int i, const long long &v) {
        for (ST[i += n] = v; i > 1; i >>= 1) pull(i >> 1);
    }

    void modify(int l, int r, const long long &v) {
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

    auto & operator[](int i) {
        return ST[i];
    }

    SegmentTree(int n, const vector<long long> &a) : n(n), h(__lg(n)), ST(2 * n), lazy(n, 0) {
        for (int i = 0; i < a.size(); i++) assign(i, a[i]);
    }
};

vector<int> sieve(int n) {
    vector<int> spf(n + 1, 0), primes;
    for (int i = 2; i <= n; i++) {
        if (!spf[i]) {
            spf[i] = i;
            primes.emplace_back(i);
        }

        for (int p : primes) {
            auto j = (long long) i * p;
            if (j > n) break;
            spf[j] = p;
            if (p == spf[i]) break;
        }
    }
    return spf;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, m;
    cin >> n >> k >> m;

    auto sum = 0LL;
    vector<int> v(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> v[i];

        sum += v[i];
    }

    vector<long long> base(bit_ceil((unsigned) n - k + 1), 0);
    for (int i = 0; i <= n - k; i++) base[i] = 1;
    SegmentTree st(base.size(), base);

    auto spf = sieve(1e5);
    auto factorize = [&](int n) {
        vector<int> pfs;
        while (n > 1) {
            int pf = spf[n];
            pfs.emplace_back(pf);
            while (!(n % pf)) n /= pf;
        }
        return pfs;
    };

    vector<set<int>> indices(1e5 + 1);
    for (int i = 1; i <= n; i++)
        for (int pf : factorize(v[i])) indices[pf].insert(i);

    auto add = [&](int sl, int sr, int d) {
        int l = max(sr - k, 0), r = min(sl, n - k + 1);
        if (l < r) st.modify(l, r, d);
    };

    for (int p = 2; p <= 1e5; p++)
        if (!indices[p].empty()) {
            int prev = -1;
            for (int i : indices[p]) {
                if (~prev) add(prev, i, 1);
                prev = i;
            }
        }

    auto fails = [&]() {
        return n - k + 1 - (!st[1].value ? st[1].freq : 0);
    };

    cout << fails() << "\n";
    while (m--) {
        int a, b;
        cin >> a >> b;

        if (v[a] != b) {
            for (int pf : factorize(v[a])) {
                auto it = indices[pf].find(a);
                int l = it == indices[pf].begin() ? 0 : *prev(it), r = next(it) == indices[pf].end() ? 0 : *next(it);
                if (l) add(l, a, -1);
                if (r) add(a, r, -1);
                if (l && r) add(l, r, 1);
                indices[pf].erase(it);
            }
            for (int pf : factorize(b)) {
                auto it = indices[pf].lower_bound(a);
                int l = it == indices[pf].begin() ? 0 : *prev(it), r = it == indices[pf].end() ? 0 : *it;
                if (l && r) add(l, r, -1);
                indices[pf].emplace(a);
                if (l) add(l, a, 1);
                if (r) add(a, r, 1);
            }
            sum += b - v[a];
            v[a] = b;
        }
        cout << fails() << "\n";
    }
    cout << sum << "\n";
}
