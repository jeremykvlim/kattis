#include <bits/stdc++.h>
using namespace std;

struct PURQSegmentTree {
    struct Monoid {
        int l, r;
        Monoid() : l(INT_MAX), r(INT_MIN) {}

        auto & operator=(const int &v) {
            if (!~v) *this = Monoid();
            else l = r = v;
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            l = min(l, monoid.l);
            r = max(r, monoid.r);
            return *this;
        }

        friend auto operator+(Monoid ml, const Monoid &mr) {
            return ml += mr;
        }
    };

    int n;
    vector<Monoid> ST;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void point_update(int i, const int &v) {
        for (ST[i += n] = v; i > 1; i >>= 1) pull(i >> 1);
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

    PURQSegmentTree(int n) : n(n), ST(2 * n) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;

    vector<int> C, D;
    vector<array<int, 3>> queries(q);
    for (auto &[t, c, d] : queries) {
        cin >> t >> c >> d;

        if (t == 1 || t == 2) {
            C.emplace_back(c);
            D.emplace_back(d);
        } else if (t == 3) {
            C.emplace_back(c);
            C.emplace_back(d);
        } else {
            D.emplace_back(c);
            D.emplace_back(d);
        }
    }
    sort(C.begin(), C.end());
    C.erase(unique(C.begin(), C.end()), C.end());
    sort(D.begin(), D.end());
    D.erase(unique(D.begin(), D.end()), D.end());

    int n = C.size(), m = D.size();
    PURQSegmentTree st_c(2 * n), st_d(2 * m);
    vector<map<int, int>> counts_c(n), counts_d(m);
    auto update = [&](const auto &counts, auto &st, int i) {
        if (counts[i].empty()) {
            st.point_update(2 * i, -1);
            st.point_update(2 * i + 1, -1);
        } else {
            st.point_update(2 * i, counts[i].begin()->first);
            st.point_update(2 * i + 1, counts[i].rbegin()->first);
        }
    };

    for (auto [t, c, d] : queries) {
        auto query = [&](const auto &v, auto &st) {
            auto [l, r] = st.range_query(2 * (lower_bound(v.begin(), v.end(), c) - v.begin()), 2 * (upper_bound(v.begin(), v.end(), d) - v.begin()));
            if (l == INT_MAX) cout << "Enginn!\n";
            else cout << l << " " << r << "\n";
        };

        if (t == 1) {
            int i = lower_bound(C.begin(), C.end(), c) - C.begin();
            counts_c[i][d]++;
            update(counts_c, st_c, i);

            int j = lower_bound(D.begin(), D.end(), d) - D.begin();
            counts_d[j][c]++;
            update(counts_d, st_d, j);
        } else if (t == 2) {
            int i = lower_bound(C.begin(), C.end(), c) - C.begin();
            counts_c[i][d]--;
            if (!counts_c[i][d]) counts_c[i].erase(d);
            update(counts_c, st_c, i);

            int j = lower_bound(D.begin(), D.end(), d) - D.begin();
            counts_d[j][c]--;
            if (!counts_d[j][c]) counts_d[j].erase(c);
            update(counts_d, st_d, j);
        } else if (t == 3) query(C, st_c);
        else query(D, st_d);
    }
}
