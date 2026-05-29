#include <bits/stdc++.h>
using namespace std;

struct RURQSegmentTree {
    struct Monoid {
        long long value, freq;

        Monoid() : value(0), freq(0) {}

        auto & operator=(const long long &v) {
            freq = v;
            return *this;
        }

        auto & operator+=(const long long &v) {
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

    RURQSegmentTree(int n, const vector<long long> &a) : n(n), h(__lg(n)), ST(2 * n), lazy(n, 0) {
        for (int i = 0; i < a.size(); i++) ST[i + n] = a[i];
        build();
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
    RURQSegmentTree st(base.size(), base);

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
        for (int pf : factorize(v[i])) indices[pf].emplace(i);

    auto update = [&](int sl, int sr, int d) {
        int l = max(sr - k, 0), r = min(sl, n - k + 1);
        if (l < r) st.range_update(l, r, d);
    };

    for (int p = 2; p <= 1e5; p++)
        if (!indices[p].empty()) {
            int prev = -1;
            for (int i : indices[p]) {
                if (~prev) update(prev, i, 1);
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
                if (l) update(l, a, -1);
                if (r) update(a, r, -1);
                if (l && r) update(l, r, 1);
                indices[pf].erase(it);
            }
            for (int pf : factorize(b)) {
                auto it = indices[pf].lower_bound(a);
                int l = it == indices[pf].begin() ? 0 : *prev(it), r = it == indices[pf].end() ? 0 : *it;
                if (l && r) update(l, r, -1);
                indices[pf].emplace(a);
                if (l) update(l, a, 1);
                if (r) update(a, r, 1);
            }
            sum += b - v[a];
            v[a] = b;
        }
        cout << fails() << "\n";
    }
    cout << sum << "\n";
}