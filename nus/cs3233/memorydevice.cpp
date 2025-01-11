#include <bits/stdc++.h>
using namespace std;

struct SegmentTree {
    struct Segment {
        int value;

        Segment() : value(INT_MAX) {}

        auto & operator=(const int &v) {
            value = v;
            return *this;
        }

        auto operator+=(const Segment &seg) {
            value = min(value, seg.value);
            return *this;
        }

        friend auto operator+(Segment sl, const Segment &sr) {
            return sl += sr;
        }
    };

    int n;
    vector<Segment> ST;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void assign(int i, const int &v) {
        for (ST[i += n] = v; i > 1; i >>= 1) pull(i >> 1);
    }

    auto range_query(int l, int r) {
        Segment seg;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) seg += ST[l++];
            if (r & 1) seg += ST[--r];
        }

        return seg;
    }

    auto & operator[](int i) {
        return ST[i];
    }

    SegmentTree(int n) : n(n), ST(2 * n) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    SegmentTree st(n + 1);
    vector<set<int>> starts(n + 1);
    set<pair<int, int>> freed;
    auto add = [&](int i, int v) {
        starts[i - v].emplace(v);
        st.assign(i - v, *starts[i - v].begin());
        freed.emplace(v, i);
    };

    auto remove = [&](int i, int v) {
        starts[i - v].erase(v);
        st.assign(i - v, *starts[i - v].begin());
        freed.erase({v, i});
    };

    for (int i = 0; i <= n; i++) starts[i].emplace(INT_MAX);
    add(n, 0);

    while (q--) {
        int c;
        cin >> c;

        if (c == 1) {
            int l;
            cin >> l;

            int p = st.range_query(l, n + 1).value;
            if (p == INT_MAX) {
                cout << "-1\n";
                continue;
            }
            cout << p << "\n";

            auto [v, i] = *freed.lower_bound({p, 0});
            remove(i, p);
            if (i > p + l) add(i, p + l);
        } else {
            int x, l;
            cin >> x >> l;

            int i = x + l;
            auto it = freed.lower_bound({x, i});
            while (it != freed.end() && it->first <= i) {
                auto [v, j] = *it;
                it = next(it);

                remove(j, v);

                l -= min(i, j) - v;
                i = max(i, j);
            }
            if (it != freed.begin() && (--it)->second >= x) {
                auto [v, j] = *it;

                remove(j, v);

                l -= min(i, j) - max(x, v);
                x = min(x, v);
                i = max(i, j);
            }
            cout << l << "\n";

            add(i, x);
        }
    }
}
