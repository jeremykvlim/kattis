#include <bits/stdc++.h>
using namespace std;

struct SegmentTree {
    struct Segment {
        vector<int> s;

        auto size() {
            return s.size();
        }

        auto & operator=(const vector<int> &v) {
            s = v;
            return *this;
        }

        auto & operator[](int i) {
            return s[i];
        }

        auto operator+=(const Segment &seg) {
            if (s.empty()) s = seg.s;
            else if (!seg.s.empty())
                for (int i = 0; i < s.size(); i++) s[i] = min(s[i], seg.s[i]);
            return *this;
        }

        friend auto operator+(Segment sl, const Segment &sr) {
            return sl += sr;
        }
    };

    int n;
    vector<Segment> ST;

    void pull(int p) {
        ST[p] = ST[p << 1] + ST[p << 1 | 1];
    }

    void assign(int i, const vector<int> &v) {
        for (ST[i += n] = v; i > 1; i >>= 1) pull(i >> 1);
    }

    Segment range_query(int l, int r) {
        Segment seg;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) seg += ST[l++];
            if (r & 1) seg += ST[--r];
        }

        return seg;
    }

    SegmentTree(int n) : n(n), ST(2 * n) {}
};

template <typename T>
struct WaveletMatrix {
    int lg;
    vector<int> mid;
    vector<vector<int>> pref;
    vector<pair<T, int>> order;

    WaveletMatrix(int n, vector<T> a) : lg(__lg(*max_element(a.begin(), a.end())) + 1), order(n) {
        mid.resize(lg);
        pref.assign(lg, vector<int>(n + 1, 0));

        vector<pair<T, int>> temp(n);
        for (int i = 0; i < n; i++) order[i] = {a[i], i};
        for (int b = lg - 1; ~b; b--) {
            for (int i = 0; i < n; i++) pref[b][i + 1] = pref[b][i] + ((order[i].first >> b) & 1);

            int zeroes = 0, ones = mid[b] = n - pref[b][n];
            for (int i = 0; i < n; i++) temp[((order[i].first >> b) & 1) ? ones++ : zeroes++] = order[i];
            order.swap(temp);
        }
    }

    tuple<T, int, int, int> quantile(int l, int r, int k) {
        T v = 0;
        for (int b = lg - 1; ~b; b--) {
            int zeroes = (r - pref[b][r]) - (l - pref[b][l]);
            if (k >= zeroes) {
                k -= zeroes;
                v |= ((T) 1) << b;
                l = pref[b][l] + mid[b];
                r = pref[b][r] + mid[b];
            } else {
                l -= pref[b][l];
                r -= pref[b][r];
            }
        }
        return {v, k, r - l, order[l + k].second};
    }

    int freq_less(int l, int r, T x) {
        int f = 0;
        for (int b = lg - 1; ~b; b--) {
            if ((x >> b) & 1) {
                f += (r - pref[b][r]) - (l - pref[b][l]);
                l = pref[b][l] + mid[b];
                r = pref[b][r] + mid[b];
            } else {
                l -= pref[b][l];
                r -= pref[b][r];
            }
        }
        return f;
    }

    int freq(int l, int r, T x) {
        if ((((T) 1) << lg) <= x) return 0;
        for (int b = lg - 1; ~b; b--) {
            if ((x >> b) & 1) {
                l = pref[b][l] + mid[b];
                r = pref[b][r] + mid[b];
            } else {
                l -= pref[b][l];
                r -= pref[b][r];
            }
        }
        return r - l;
    }

    T lower_bound(int l, int r, T x) {
        int f = freq_less(l, r, x);
        return f == r - l ? -1 : get<0>(quantile(l, r, f));
    }

    T predecessor(int l, int r, T x) {
        int f = freq_less(l, r, x);
        return !f ? -1 : get<0>(quantile(l, r, f - 1));
    }

    T successor(int l, int r, T x) {
        int f = freq_less(l, r, x) + freq(l, r, x);
        return f == r - l ? -1 : get<0>(quantile(l, r, f));
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, q;
        cin >> n >> q;

        vector<int> a(n);
        map<int, int> compress;
        for (int &ai : a) {
            cin >> ai;

            compress[ai];
        }

        int count = 0;
        for (auto &[ai, i] : compress) i = count++;

        vector<tuple<int, int, int, int>> queries(q);
        vector<array<int, 3>> k_order_statistics(2 * q);
        for (int i = 0; i < q; i++) {
            int l, r, u, v;
            cin >> l >> r >> u >> v;

            queries[i] = {l - 1, r, u, v};
            k_order_statistics[2 * i] = {l - 1, r, u};
            k_order_statistics[2 * i + 1] = {l - 1, r, v};
        }

        WaveletMatrix wm(n, a);
        vector<vector<int>> OR(q, vector<int>(3));
        vector<vector<tuple<int, int, int, int>>> subranges(n);
        for (int i = 0; i < q; i++) {
            auto [l, r, u, v] = queries[i];

            auto [val1, o1, len1, pos1] = wm.quantile(l, r, u - 1);
            auto [val2, o2, len2, pos2] = wm.quantile(l, r, v - 1);

            int freq1 = len1 - o1, freq2 = o2 + 1;
            for (int j = 0; j < min({3, freq1, v - u + 1}); j++) OR[i][j] |= val1;
            for (int j = 0; j < min({3, freq2, v - u + 1}); j++) OR[i][j] |= val2;

            subranges[l].emplace_back(r, compress[val1] + 1, compress[val2], i);
        }

        vector<SegmentTree> sts(3, SegmentTree(n));
        vector<vector<int>> appearances(compress.size());
        for (int l = n - 1; ~l; l--) {
            int i = compress[a[l]];
            appearances[i].emplace_back(l);

            for (int j = 0; j < 3 && j < appearances[i].size(); j++) {
                vector<int> s(wm.lg, INT_MAX);
                for (int k = 0; k < wm.lg; k++)
                    if ((a[l] >> k) & 1) s[k] = appearances[i][appearances[i].size() - j - 1];

                sts[j].assign(i, s);
            }

            for (auto [r, u, v, i] : subranges[l]) {
                if (u >= v) continue;
                for (int j = 0; j < 3; j++) {
                    auto s = sts[j].range_query(u, v);
                    for (int k = 0; k < s.size(); k++)
                        if (s[k] < r) OR[i][j] |= 1 << k;
                }
            }
        }

        for (auto f : OR) cout << (long long) f[0] + f[1] + f[2] << "\n";
    }
}