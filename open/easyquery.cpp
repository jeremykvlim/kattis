#include <bits/stdc++.h>
using namespace std;

struct SegmentTree {
    struct Segment {
        vector<int> s;

        auto size() {
            return s.size();
        }

        auto & operator=(vector<int> &v) {
            s = v;
            return *this;
        }

        auto & operator[](int i) {
            return s[i];
        }

        auto operator+=(Segment seg) {
            if (s.empty()) s = seg.s;
            else if (!seg.s.empty())
                for (int i = 0; i < s.size(); i++) s[i] = min(s[i], seg.s[i]);
            return *this;
        }

        friend auto operator+(Segment sl, Segment sr) {
            return sl += move(sr);
        }
    };

    int n;
    vector<Segment> ST;

    void pull(int p) {
        ST[p] = ST[p << 1] + ST[p << 1 | 1];
    }

    void assign(int i, vector<int> &v) {
        for (ST[i += n] = v; i > 1; i >>= 1) pull(i >> 1);
    }

    auto query(int l, int r) {
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
struct WaveletTree {
    vector<T> WT, temp;
    vector<int> pref1, pref2;
    int b;

    void range_select(vector<array<int, 3>> &order_statistics) {
        for (int bit = b; ~bit; bit--) {
            for (int i = 0; i < WT.size(); i++) {
                pref1[i + 1] = pref1[i] + !((WT[i] >> bit) & 1);
                pref2[i + 1] = pref2[i] + ((WT[i] >> bit) & 1);
            }

            int it1 = 0, it2 = pref1.back();
            for (int e : WT) temp[!((e >> bit) & 1) ? it1++ : it2++] = e;
            swap(WT, temp);

            for (auto &[l, r, k] : order_statistics) {
                if (r - l < k || !k) continue;

                int K = pref1[r] - pref1[l];
                if (k <= K) {
                    l = pref1[l];
                    r = pref1[r];
                } else {
                    k -= K;
                    l = pref1.back() + pref2[l];
                    r = pref1.back() + pref2[r];
                }
            }
        }
    }

    auto operator[](int i) {
        return WT[i];
    }

    WaveletTree(vector<T> a, vector<array<int, 3>> &order_statistics) : WT(a.begin(), a.end()),
                                                                        temp(a.size()),
                                                                        pref1(a.size() + 1),
                                                                        pref2(a.size() + 1) {
        b = __lg(*max_element(a.begin(), a.end()));
        range_select(order_statistics);
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
        vector<array<int, 3>> order_statistics(2 * q);
        for (int i = 0; i < q; i++) {
            int l, r, u ,v;
            cin >> l >> r >> u >> v;

            queries[i] = {l - 1, r, u, v};
            order_statistics[2 * i] = {l - 1, r, u};
            order_statistics[2 * i + 1] = {l - 1, r, v};
        }

        WaveletTree<int> wt(a, order_statistics);
        vector<vector<int>> OR(q, vector<int>(3));
        vector<vector<tuple<int, int, int, int>>> subranges(n);
        for (int i = 0; i < q; i++) {
            auto [l, r, u, v] = queries[i];
            auto [l1, r1, k1] = order_statistics[2 * i];
            auto [l2, r2, k2] = order_statistics[2 * i + 1];

            int freq1 = r1 - l1 + 1 - k1, freq2 = k2;
            for (int j = 0; j < min({3, freq1, v - u + 1}); j++) OR[i][j] |= wt[r1 - 1];
            for (int j = 0; j < min({3, freq2, v - u + 1}); j++) OR[i][j] |= wt[r2 - 1];

            subranges[l].emplace_back(r, compress[wt[r1 - 1]] + 1, compress[wt[r2 - 1]], i);
        }

        vector<SegmentTree> sts(3, SegmentTree(n));
        vector<vector<int>> appearances(compress.size());
        for (int l = n - 1; ~l; l--) {
            int i = compress[a[l]];
            appearances[i].emplace_back(l);

            for (int j = 0; j < 3 && j < appearances[i].size(); j++) {
                vector<int> s(wt.b + 1, INT_MAX);
                for (int k = 0; k <= wt.b; k++)
                    if (a[l] & (1 << k)) s[k] = appearances[i][appearances[i].size() - j - 1];

                sts[j].assign(i, s);
            }

            for (auto [r, u, v, i] : subranges[l]) {
                if (u >= v) continue;
                for (int j = 0; j < 3; j++) {
                    auto s = sts[j].query(u, v);
                    for (int k = 0; k < s.size(); k++)
                        if (s[k] < r) OR[i][j] |= 1 << k;
                }
            }
        }

        for (auto f : OR) cout << (long long) f[0] + f[1] + f[2] << "\n";
    }
}
