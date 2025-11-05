#include <bits/stdc++.h>
using namespace std;

struct SegmentTree {
    struct Segment {
        long long value, freq;

        Segment() : value(0), freq(0) {}

        auto & operator=(const int &v) {
            freq = v;
            return *this;
        }

        auto operator+=(const int &v) {
            value += v;
            return *this;
        }

        auto operator+=(const Segment &seg) {
            if (value > seg.value) return *this;
            else if (value < seg.value) return seg;
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

    void assign(int i, const int &v) {
        for (ST[i += n] = v; i > 1; i >>= 1) pull(i >> 1);
    }

    void modify(int l, int r, const int &v) {
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

    SegmentTree(int n, const vector<int> &a) : n(n), h(__lg(n)), ST(2 * n), lazy(n, 0) {
        for (int i = 0; i < a.size(); i++) assign(i, a[i]);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int c;
    cin >> c;

    while (c--) {
        int n;
        cin >> n;

        vector<array<int, 4>> sweep;
        vector<int> ys;
        while (n--) {
            int x1, y1, x2, y2, s;
            cin >> x1 >> y1 >> x2 >> y2 >> s;

            if (x1 == x2 || y1 == y2) continue;
            sweep.push_back({x1, y1, y2, s});
            sweep.push_back({x2, y1, y2, -s});
            ys.emplace_back(y1);
            ys.emplace_back(y2);
        }

        if (sweep.empty()) {
            cout << "0 0\n";
            continue;
        }

        sort(sweep.begin(), sweep.end());
        sort(ys.begin(), ys.end());
        ys.erase(unique(ys.begin(), ys.end()), ys.end());
        vector<int> diff(bit_ceil(ys.size()), 0);
        for (int i = 0; i < ys.size() - 1; i++) diff[i] = ys[i + 1] - ys[i];

        SegmentTree st(diff.size(), diff);
        long long coverage = 0, area = 0;
        for (int i = 0; i < sweep.size();) {
            int x = sweep[i][0];

            auto search = [&](int y) {
                return lower_bound(ys.begin(), ys.end(), y) - ys.begin();
            };
            for (; i < sweep.size() && sweep[i][0] == x; i++) st.modify(search(sweep[i][1]), search(sweep[i][2]), sweep[i][3]);

            if (coverage < st[1].value) {
                coverage = st[1].value;
                area = 0;
            }
            if (coverage == st[1].value && i < sweep.size()) area += (sweep[i][0] - x) * st[1].freq;
        }
        cout << coverage << " " << area << "\n";
    }
}
