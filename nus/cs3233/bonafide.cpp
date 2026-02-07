#include <bits/stdc++.h>
using namespace std;

struct HashedString {
    static inline unsigned long long B1 = 0, B2 = 0;
    static const unsigned long long MOD1 = 1e9 + 7, MOD2 = 1e9 + 9;

    int n;
    vector<unsigned long long> pref1, pref2;
    static inline vector<unsigned long long> p1{1}, p2{1};

    HashedString() : n(0), pref1(1, 0), pref2(1, 0) {}
    HashedString(const string &s) : n(s.size()), pref1(n + 1, 0), pref2(n + 1, 0) {
        if (!B1 && !B2) {
            mt19937_64 rng{random_device{}()};
            B1 = uniform_int_distribution(911382323ULL, MOD1 - 1)(rng);
            B2 = uniform_int_distribution(972663749ULL, MOD2 - 1)(rng);
        }
        while (p1.size() <= n || p2.size() <= n) {
            p1.emplace_back((p1.back() * B1) % MOD1);
            p2.emplace_back((p2.back() * B2) % MOD2);
        }
        for (int i = 0; i < n; i++) {
            auto v = (unsigned char) s[i] + 1;
            pref1[i + 1] = (pref1[i] * B1 + v) % MOD1;
            pref2[i + 1] = (pref2[i] * B2 + v) % MOD2;
        }
    }

    pair<unsigned long long, unsigned long long> pref_hash(int l, int r) const {
        auto h1 = (pref1[r] + MOD1 - (pref1[l] * p1[r - l]) % MOD1) % MOD1, h2 = (pref2[r] + MOD2 - (pref2[l] * p2[r - l]) % MOD2) % MOD2;
        return {h1, h2};
    }

    pair<unsigned long long, unsigned long long> split_pref_hash(int i) const {
        auto [ll, lr] = pref_hash(0, i);
        auto [rl, rr] = pref_hash(i + 1, n);
        return {(ll * p1[n - i - 1] + rl) % MOD1, (lr * p2[n - i - 1] + rr) % MOD2};
    }
};

template <typename T>
struct FenwickTree {
    vector<T> BIT;

    void update(int i, T v) {
        for (; i && i < BIT.size(); i += i & -i) BIT[i] += v;
    }

    T pref_sum(int i) {
        T sum = 0;
        for (; i; i &= i - 1) sum += BIT[i];
        return sum;
    }

    T range_sum_query(int l, int r) {
        if (l >= r) return 0;
        return pref_sum(r) - pref_sum(l);
    }

    FenwickTree(int n) : BIT(n, 0) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, Q;
    string s;
    cin >> n >> Q >> s;
    n = 2 * n + 1;

    string t(n, '#');
    for (int i = 0; i < n; i++)
        if (i & 1) t[i] = s[i / 2];

    auto rev = t;
    reverse(rev.begin(), rev.end());
    auto both = t + '_' + rev;
    HashedString hs(both);

    vector<int> radius0(n), radius1(n), radius2(n), s0(n), s1(n), s2(n);
    for (int i = 0; i < n; i++) {
        int bound = min(i, n - 1 - i);
        auto lce = [&](int offset) -> int {
            int l = 0, r = bound - offset + 2, m;
            while (l + 1 < r) {
                m = l + (r - l) / 2;

                if (hs.pref_hash(2 * n - (i - offset), 2 * n - (i - offset) + m) == hs.pref_hash(i + offset, i + offset + m)) l = m;
                else r = m;
            }
            return l;
        };

        int r0 = lce(1), r1 = r0;
        char mismatch1_l = '-', mismatch1_r = '-';
        if (r0 < bound) {
            mismatch1_l = t[i - r0 - 1];
            mismatch1_r = t[i + r0 + 1];
            r1 += lce(r0 + 2) + 1;
        }

        int r2 = r1;
        char mismatch2_l = '-', mismatch2_r = '-';
        if (r1 < bound) {
            mismatch2_l = t[i - r1 - 1];
            mismatch2_r = t[i + r1 + 1];
            r2 += lce(r1 + 2) + 1;
        }

        radius0[i] = r0 + 1;
        radius1[i] = r1 + 1;
        radius2[i] = r2 + 1;

        bool b0 = true, b1 = false, b2 = false;
        if (r0 < bound)
            if (t[i] == mismatch1_l || t[i] == mismatch1_r) b1 = true;

        if (r1 < bound)
            if ((mismatch1_l == mismatch2_l && mismatch1_r == mismatch2_r) || (mismatch1_l == mismatch2_r && mismatch1_r == mismatch2_l)) b2 = true;

        s0[i] = b0 - b1;
        s1[i] = b1 - b2;
        s2[i] = b2;
    }

    vector<pair<int, int>> queries(Q);
    for (auto &[l, r] : queries) {
        cin >> l >> r;
        l = 2 * (l - 1);
        r = 2 * (r - 1) + 2;
    }

    auto sum = [&](int l, int r) {
        return (long long) (l + r) * (r - l + 1) / 2;
    };

    vector<array<long long, 5>> ql, qr;
    for (int i = 0; i < Q; i++) {
        auto [l, r] = queries[i];
        int m = l + (r - l) / 2;

        if (l <= m) ql.push_back({l, m, 1 - l, i, sum(l, m)});
        if (m + 1 <= r) qr.push_back({m + 1, r, r + 1, i, -sum(m + 1, r)});
    }

    vector<int> indices_l(ql.size()), indices_r(qr.size());
    iota(indices_l.begin(), indices_l.end(), 0);
    sort(indices_l.begin(), indices_l.end(), [&](int i, int j) {
        if (ql[i][2] != ql[j][2]) return ql[i][2] < ql[j][2];
        return i < j;
    });
    iota(indices_r.begin(), indices_r.end(), 0);
    sort(indices_r.begin(), indices_r.end(), [&](int i, int j) {
        if (qr[i][2] != qr[j][2]) return qr[i][2] < qr[j][2];
        return i < j;
    });

    vector<long long> count(Q, 0);
    vector<int> left(n), right(n), order(n);
    auto process = [&](const auto &active, int sgn) {
        for (int i = 0; i < n; i++) {
            left[i] = active[i] - i;
            right[i] = active[i] + i;
        }

        auto sweep = [&](const auto &half, const auto &q, const auto &indices) {
            iota(order.begin(), order.end(), 0);
            sort(order.begin(), order.end(), [&](int a, int b) {
                if (half[a] != half[b]) return half[a] < half[b];
                return a < b;
            });

            FenwickTree<long long> fw_a(n + 1), fw_b(n + 1);
            for (int i = 0; i < n; i++) fw_b.update(i + 1, 1);

            int j = 0;
            for (int k : indices) {
                auto [l, r, c, i, base] = q[k];
                for (; j < n && half[order[j]] <= c; j++) {
                    int h = order[j];
                    fw_a.update(h + 1, half[h]);
                    fw_b.update(h + 1, -1);
                }
                count[i] += sgn * (fw_a.range_sum_query(l, r + 1) + c * (fw_b.range_sum_query(l, r + 1)) + base);
            }
        };
        sweep(left, ql, indices_l);
        sweep(right, qr, indices_r);

        vector<int> pref(n + 1);
        for (int i = 0; i < n; i++) pref[i + 1] = pref[i] + (!(i & 1) && active[i] > 0);
        for (int i = 0; i < Q; i++) {
            auto [l, r] = queries[i];
            count[i] -= sgn * (pref[r + 1] - pref[l]);
        }
    };

    auto apply = [&](const vector<int> &radius, const auto &sign) {
        vector<int> active(n);
        auto eval = [&](int sgn) {
            for (int i = 0; i < n; i++) active[i] = sign[i] == sgn ? radius[i] : 0;
            process(active, sgn);
        };
        eval(1);
        eval(-1);
    };
    apply(radius0, s0);
    apply(radius1, s1);
    apply(radius2, s2);

    for (auto c : count) cout << c / 2 << "\n";
}
