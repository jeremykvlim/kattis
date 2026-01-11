#include <bits/stdc++.h>
using namespace std;

struct PURQSegmentTree {
    struct Monoid {
        array<int, 3> vals;

        Monoid() : vals{0, 0, 0} {}

        auto & operator=(const int &v) {
            vals = {v, v, v};
            return *this;
        }

        auto & operator+=(const int &v) {
            vals[0] |= v;
            vals[1] |= v;
            vals[2] |= v;
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            vals[0] |= monoid.vals[0];
            vals[1] |= monoid.vals[1];
            vals[2] |= monoid.vals[2];
            return *this;
        }

        friend auto operator+(Monoid ml, const Monoid &mr) {
            return ml += mr;
        }
    };

    int n;
    vector<Monoid> ST;
    vector<bool> visited;
    stack<int> undo;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void visit(int i) {
        if (!visited[i]) {
            visited[i] = true;
            undo.emplace(i);
        }
    }

    void apply(int i, const int &v, int k) {
        visit(i);
        ST[i].vals[k] |= v;
    }

    void point_update(int i, const int &v, int k) {
        for (apply(i += n, v, k); i > 1; i >>= 1) {
            int p = i >> 1;
            visit(p);
            pull(p);
        }
    }

    Monoid range_query(int l, int r) {
        Monoid ml, mr;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) ml = ml + ST[l++];
            if (r & 1) mr = ST[--r] + mr;
        }
        return ml + mr;
    }

    auto & operator[](int i) {
        return ST[i];
    }

    PURQSegmentTree(int n) : n(n), ST(2 * n), visited(2 * n, false) {}

    void reset() {
        while (!undo.empty()) {
            int i = undo.top();
            ST[i] = Monoid();
            visited[i] = false;
            undo.pop();
        }
    }
};

template <typename T>
struct WaveletMatrix {
    int lg;
    vector<int> mid;
    vector<vector<int>> pref;
    vector<pair<T, int>> order;

    WaveletMatrix(int n, vector<T> a) : lg(__lg(*max_element(a.begin(), a.end()) + 1) + 1), order(n) {
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
        for (int &ai : a) cin >> ai;

        auto deduped = a;
        sort(deduped.begin(), deduped.end());
        deduped.erase(unique(deduped.begin(), deduped.end()), deduped.end());
        for (int i = 0; i < n; i++) a[i] = lower_bound(deduped.begin(), deduped.end(), a[i]) - deduped.begin();

        WaveletMatrix<int> wm(n, a);
        vector<array<int, 3>> OR(q, {0, 0, 0});
        vector<array<int, 6>> sweep;
        for (int i = 0; i < q; i++) {
            int l, r, u, v;
            cin >> l >> r >> u >> v;
            u--;
            v--;

            auto [val1, o1, len1, pos1] = wm.quantile(l - 1, r, u);
            auto [val2, o2, len2, pos2] = wm.quantile(l - 1, r, v);
            if (val1 == val2) {
                for (int j = 0; j < min(3, v - u + 1); j++) OR[i][j] |= deduped[val1];
                continue;
            }
            for (int j = 0; j < min(3, len1 - o1); j++) OR[i][j] |= deduped[val1];
            for (int j = 0; j < min(3, o2 + 1); j++) OR[i][j] |= deduped[val2];
            if (val1 + 1 < val2) sweep.push_back({l - 1, r - 1, val1 + 1, val2 - 1, -(i + 1)});
        }

        int m = deduped.size();
        vector<int> prev0(m, -1), prev1(m, -1), prev2(m, -1);
        for (int i = 0; i < n; i++) {
            int ai = a[i];
            prev2[ai] = prev1[ai];
            prev1[ai] = prev0[ai];
            prev0[ai] = i;

            sweep.push_back({i, i, ai, deduped[ai], 0});
            if (~prev1[ai]) sweep.push_back({prev1[ai], i, ai, deduped[ai], 1});
            if (~prev2[ai]) sweep.push_back({prev2[ai], i, ai, deduped[ai], 2});
        }
        sort(sweep.begin(), sweep.end(), [&](const auto &a1, const auto &a2) { return a1[0] != a2[0] ? a1[0] > a2[0] : (a1[1] != a2[1] ? a1[1] < a2[1] : a1[4] > a2[4]); });
        for (int i = 0; i < sweep.size(); i++) sweep[i][5] = i;

        PURQSegmentTree st(m);
        auto dnc = [&](auto &&self, int l, int r) -> void {
            if (l + 1 == r) return;

            int mid = l + (r - l) / 2;
            self(self, l, mid);
            self(self, mid, r);

            stable_sort(sweep.begin() + l, sweep.begin() + r, [&](const auto &a1, const auto &a2) { return a1[1] < a2[1]; });
            for (int i = l; i < r; i++) {
                auto [_, __, u, v, k, pos] = sweep[i];
                if (k >= 0 && pos < mid) st.point_update(u, v, k);
                else if (k < 0 && pos >= mid) {
                    auto vals = st.range_query(u, v + 1).vals;
                    OR[-(k + 1)][0] |= vals[0];
                    OR[-(k + 1)][1] |= vals[1];
                    OR[-(k + 1)][2] |= vals[2];
                }
            }

            st.reset();
        };
        dnc(dnc, 0, sweep.size());

        for (auto [ft1, ft2, ft3] : OR) cout << (long long) ft1 + ft2 + ft3 << "\n";
    }
}
