#include <bits/stdc++.h>
using namespace std;

struct SegmentTree {
    struct Segment {
        int value, freq;

        Segment() : value(0), freq(0) {}

        auto & operator=(const int &v) {
            value = v;
            freq = 1;
            return *this;
        }

        auto operator+=(const int &v) {
            value += v;
            return *this;
        }

        auto & operator+=(const Segment &seg) {
            if (value == seg.value) freq += seg.freq;
            else if (freq > seg.freq) freq -= seg.freq;
            else {
                value = seg.value;
                freq = seg.freq - freq;
            }

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

    Segment range_query(int l, int r) {
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

    SegmentTree(int n, const vector<int> &a) : n(n), ST(2 * n) {
        for (int i = 0; i < a.size(); i++) assign(i, a[i]);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, c;
    cin >> n >> c;

    vector<int> colors(n + 1);
    vector<vector<int>> indices(c + 1);
    for (int i = 0; i < n; i++) {
        cin >> colors[i];
        
        indices[colors[i]].emplace_back(i + 1);
    }
    SegmentTree st(bit_ceil((unsigned) n), colors);

    int m;
    cin >> m;

    while (m--) {
        int a, b;
        cin >> a >> b;

        int color = st.range_query(a - 1, b).value;
        if (!color) cout << "no\n";
        else {
            if (upper_bound(indices[color].begin(), indices[color].end(), b) - lower_bound(indices[color].begin(), indices[color].end(), a) > (b - a + 1) / 2) cout << "yes " << color << "\n";
            else cout << "no\n";
        }
    }
}
