#include <bits/stdc++.h>
using namespace std;

struct SegmentTree {
    long long K;

    struct Segment {
        vector<pair<long long, int>> pref, suff;
        int len, dp;
        long long *K;

        Segment(long long &K) : len(0), dp(INT_MAX), K(&K) {}

        auto & operator=(long long v) {
            len = 1;
            dp = (v == *K ? 1 : INT_MAX);
            pref = {{v, 1}};
            suff = {{v, 1}};
            return *this;
        }

        friend auto operator+(Segment &sl, Segment &sr) {
            Segment seg(*sl.K);

            auto add = [](vector<pair<long long, int>> &aff1, vector<pair<long long, int>> &aff2, vector<pair<long long, int>> &aff3, int len) {
                for (auto aff : aff2) aff1.emplace_back(aff);

                auto mask = aff2.back().first;
                for (auto [m, l] : aff3) {
                    if ((mask | m) == mask) continue;
                    mask |= m;
                    aff1.emplace_back(mask, len + l);
                }
            };
            if (!sl.pref.empty()) add(seg.pref, sl.pref, sr.pref, sl.len);
            if (!sr.suff.empty()) add(seg.suff, sr.suff, sl.suff, sr.len);

            for (int i = sl.suff.size() - 1, j = 0; ~i; i--) {
                while (j < sr.pref.size() && (sl.suff[i].first | sr.pref[j].first) != *seg.K) j++;
                if (j == sr.pref.size() || (sl.suff[i].first | sr.pref[j].first) != *seg.K) break;
                
                seg.dp = min(seg.dp, sl.suff[i].second + sr.pref[j].second);
            }

            seg.dp = min({seg.dp, sl.dp, sr.dp});
            seg.len = sl.len + sr.len;
            return seg;
        }
    };

    int n;
    vector<Segment> ST;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void build() {
        for (int i = n - 1; i; i--) pull(i);
    }

    void assign(int i, long long v) {
        for (ST[i += n] = v; i > 1; i >>= 1) pull(i >> 1);
    }

    auto & operator[](int i) {
        return ST[i];
    }

    SegmentTree(int n, long long k, vector<int> &a) : n(n), K(k), ST(2 * n, Segment(K)) {
        for (int i = 0; i < a.size(); i++) assign(i + 1, 1LL << a[i]);
        build();
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, m;
    cin >> n >> k >> m;

    vector<int> a(n);
    for (int &ai : a) {
        cin >> ai;

        ai--;
    }

    SegmentTree st(1 << (__lg(n) + 1), (1LL << k) - 1, a);
    while (m--) {
        int q;
        cin >> q;

        if (q == 1) {
            int p, v;
            cin >> p >> v;

            st.assign(p, 1LL << (v - 1));
        } else cout << (st[1].dp == INT_MAX ? -1 : st[1].dp) << "\n";
    }
}
