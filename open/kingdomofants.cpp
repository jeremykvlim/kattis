#include <bits/stdc++.h>
using namespace std;

struct SegmentTree {
    struct Segment {
        long long value, freq, even, odd;

        Segment() : value(0), freq(0), even(0), odd(0) {}

        auto & operator=(const long long &v) {
            freq = even = v;
            odd = 0;
            return *this;
        }

        auto operator+=(const long long &v) {
            value += v;
            if (v & 1) swap(even, odd);
            return *this;
        }

        auto operator+=(const Segment &seg) {
            if (value < seg.value);
            else if (value > seg.value) {
                value = seg.value;
                freq = seg.freq;
            } else freq += seg.freq;
            
            even += seg.even;
            odd  += seg.odd;
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

    void apply(int i, const long long &v) {
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

    void assign(int i, const long long &v) {
        for (ST[i += n] = v; i > 1; i >>= 1) pull(i >> 1);
    }

    void modify(int l, int r, const long long &v) {
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

    SegmentTree(int n, const vector<long long> &a) : n(n), h(__lg(n)), ST(2 * n), lazy(n, 0) {
        for (int i = 0; i < a.size(); i++) assign(i, a[i]);
    }
};

template <typename T>
unsigned long long even_area_of_union_of_rectangles(vector<array<T, 4>> rectangles) {
    if (rectangles.empty()) return 0;

    int n = rectangles.size();
    vector<pair<T, int>> y(2 * n);
    for (int i = 0; i < n; i++) {
        auto [xl, xr, yd, yu] = rectangles[i];
        y[2 * i] = {yd, 2 * i};
        y[2 * i + 1] = {yu, 2 * i + 1};
    }
    sort(y.begin(), y.end());

    vector<T> coords(2 * n);
    for (int i = 0; i < 2 * n; i++) coords[i] = y[i].first;
    coords.erase(unique(coords.begin(), coords.end()), coords.end());

    vector<T> y_gaps(coords.size() - 1);
    for (int i = 0; i < coords.size() - 1; i++) y_gaps[i] = coords[i + 1] - coords[i];
    for (auto [yi, i] : y) rectangles[i / 2][2 + (i & 1)] = lower_bound(coords.begin(), coords.end(), yi) - coords.begin();

    vector<array<T, 4>> sweep(2 * n);
    for (int i = 0; i < n; i++) {
        auto [xl, xr, yd, yu] = rectangles[i];
        sweep[2 * i] = {xl, yd, yu, 2 * i};
        sweep[2 * i + 1] = {xr, yd, yu, 2 * i + 1};
    }
    sort(sweep.begin(), sweep.end());

    auto a = 0ULL, a_odd = 0ULL;
    SegmentTree st(y_gaps.size(), y_gaps);
    T y_range = y.back().first - y.front().first, prev = sweep[0][0];
    for (auto [x, yd, yu, i] : sweep) {
        if (prev != x) {
            auto [v, f, even, odd] = st[1];

            if (!v) a += (y_range - f) * (x - prev);
            else a += y_range * (x - prev);
            a_odd += odd * (x - prev);

            prev = x;
        }
        st.modify(yd, yu, i & 1 ? -1 : 1);
    }
    return a - a_odd;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<array<long long, 4>> rectangles;
    for (int i = 0; i < n; i++) {
        long long x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;

        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);
        if (x1 < x2 && y1 < y2) rectangles.push_back({x1, x2, y1, y2});
    }
    cout << even_area_of_union_of_rectangles(rectangles);
}
