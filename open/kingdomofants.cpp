#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct RURQSegmentTree {
    struct Monoid {
        int value;
        T freq, even, odd;

        Monoid() : value(0), freq(0), even(0), odd(0) {}

        auto & operator=(const T &v) {
            freq = even = v;
            return *this;
        }

        auto & operator+=(const int &v) {
            value += v;
            if (v & 1) swap(even, odd);
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            if (value > monoid.value) {
                value = monoid.value;
                freq = monoid.freq;
            } else if (value == monoid.value) freq += monoid.freq;

            even += monoid.even;
            odd += monoid.odd;
            return *this;
        }

        friend auto operator+(Monoid ml, const Monoid &mr) {
            ml += mr;
            return ml;
        }
    };

    int n, h;
    vector<Monoid> ST;
    vector<int> lazy;

    void pull(int i) {
        ST[i] = ST[i << 1] + ST[i << 1 | 1];
    }

    void build() {
        for (int i = n - 1; i; i--) pull(i);
    }

    void apply(int i, const int &v) {
        ST[i] += v;
        if (i < n) lazy[i] += v;
    }

    void push(int i) {
        if (lazy[i]) {
            apply(i << 1, lazy[i]);
            apply(i << 1 | 1, lazy[i]);
            lazy[i] = 0;
        }
    }

    void push_down(int l, int r) {
        for (int b = h; b; b--) {
            if (((l >> b) << b) != l) push(l >> b);
            if (((r >> b) << b) != r) push((r - 1) >> b);
        }
    }

    void pull_up(int l, int r) {
        for (int b = 1; b <= h; b++) {
            if (((l >> b) << b) != l) pull(l >> b);
            if (((r >> b) << b) != r) pull((r - 1) >> b);
        }
    }

    void range_update(int l, int r, const int &v) {
        l += n;
        r += n;
        push_down(l, r);

        int temp_l = l, temp_r = r;
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) apply(l++, v);
            if (r & 1) apply(--r, v);
        }

        pull_up(temp_l, temp_r);
    }

    Monoid range_query(int l, int r) {
        l += n;
        r += n;
        push_down(l, r);

        Monoid ml, mr;
        for (; l < r; l >>= 1, r >>= 1) {
            if (l & 1) ml = ml + ST[l++];
            if (r & 1) mr = ST[--r] + mr;
        }

        return ml + mr;
    }

    auto & operator[](int i) {
        return ST[i];
    }

    RURQSegmentTree(int n, const vector<T> &a) : n(n), h(__lg(n)), ST(2 * n), lazy(n, 0) {
        for (int i = 0; i < a.size(); i++) ST[i + n] = a[i];
        build();
    }
};

template <typename T>
T area_of_union_of_even_number_of_rectangles(const vector<array<T, 4>> &rectangles) {
    if (rectangles.empty()) return 0;

    int n = rectangles.size();
    vector<T> ys(2 * n);
    for (int i = 0; i < n; i++) {
        auto [xl, xr, yd, yu] = rectangles[i];
        ys[2 * i] = yd;
        ys[2 * i + 1] = yu;
    }
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());

    vector<T> y_gaps(ys.size() - 1);
    for (int i = 0; i < ys.size() - 1; i++) y_gaps[i] = ys[i + 1] - ys[i];

    vector<tuple<T, int, int, int>> sweep(2 * n);
    for (int i = 0; i < n; i++) {
        auto [xl, xr, yd, yu] = rectangles[i];
        int l = lower_bound(ys.begin(), ys.end(), yd) - ys.begin(), r = lower_bound(ys.begin(), ys.end(), yu) - ys.begin();
        sweep[2 * i] = {xl, l, r, 1};
        sweep[2 * i + 1] = {xr, l, r, -1};
    }
    sort(sweep.begin(), sweep.end());

    RURQSegmentTree st(bit_ceil(y_gaps.size()), y_gaps);
    T a = 0, prev = get<0>(sweep[0]);
    for (auto [x, yd, yu, delta] : sweep) {
        if (prev != x) {
            auto [v, f, even, odd] = st[1];

            a += (even - (!v ? f : 0)) * (x - prev);

            prev = x;
        }
        st.range_update(yd, yu, delta);
    }
    return a;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<array<long long, 4>> rectangles;
    while (n--) {
        long long x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;

        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);
        if (x1 < x2 && y1 < y2) rectangles.push_back({x1, x2, y1, y2});
    }
    cout << area_of_union_of_even_number_of_rectangles(rectangles);
}