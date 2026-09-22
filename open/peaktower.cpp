#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct RURQSegmentTree {
    struct Monoid {
        int value;
        T freq;

        Monoid() : value(0), freq(0) {}

        auto & operator=(const T &v) {
            freq = v;
            return *this;
        }

        auto & operator+=(const int &v) {
            value += v;
            return *this;
        }

        auto & operator+=(const Monoid &monoid) {
            if (value < monoid.value) return *this;
            if (value > monoid.value) return *this = monoid;
            freq += monoid.freq;
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
T area_of_union_of_rectangles(const vector<array<T, 4>> &rectangles) {
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
    T a = 0, y_range = ys.back() - ys.front(), prev = get<0>(sweep[0]);
    for (auto [x, yd, yu, delta] : sweep) {
        if (prev != x) {
            auto [v, f] = st[1];

            if (!v) a += (y_range - f) * (x - prev);
            else a += y_range * (x - prev);

            prev = x;
        }
        st.range_update(yd, yu, delta);
    }
    return a;
}

template <typename T>
vector<complex<T>> quadratic_roots(T a, T b, T c) {
    if (fabs(a) < 1e-8 && fabs(b) < 1e-8) return {};
    if (fabs(a) < 1e-8) return {(-c / b)};

    complex<T> discriminant(b * b - 4 * a * c, 0);
    return {(-b + sqrt(discriminant)) / (2 * a), (-b - sqrt(discriminant)) / (2 * a)};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    double W, H, E;
    cin >> n >> W >> H >> E;

    vector<array<double, 6>> objects(n);
    vector<pair<double, double>> xs, ys;
    for (auto &[w, h, sx, sy, vx, vy] : objects) {
        cin >> w >> h >> sx >> sy >> vx >> vy;

        xs.emplace_back(sx, vx);
        xs.emplace_back(sx + w, vx);
        ys.emplace_back(sy, vy);
        ys.emplace_back(sy + h, vy);
    }
    xs.emplace_back(0, 0);
    xs.emplace_back(W, 0);
    ys.emplace_back(0, 0);
    ys.emplace_back(H, 0);

    vector<double> times{0, E};
    auto add_events = [&](const auto &coords) {
        for (int i = 0; i < coords.size(); i++)
            for (int j = i + 1; j < coords.size(); j++) {
                auto [si, vi] = coords[i];
                auto [sj, vj] = coords[j];

                if (vi != vj) {
                    auto t = (sj - si) / (vi - vj);
                    if (0 < t && t < E) times.emplace_back(t);
                }
            }
    };
    add_events(xs);
    add_events(ys);
    sort(times.begin(), times.end());
    vector<double> temp;
    for (auto t : times)
        if (temp.empty() || fabsl(t - temp.back()) > 1e-14L) temp.emplace_back(t);
    times = temp;

    auto area = [&](double t) {
        vector<array<double, 4>> rectangles;
        for (auto [w, h, sx, sy, vx, vy] : objects) {
            auto xl = max(0., sx + vx * t), xr = min(W, sx + vx * t + w), yd = max(0., sy + vy * t), yu = min(H, sy + vy * t + h);
            if (xl < xr && yd < yu) rectangles.push_back({xl, xr, yd, yu});
        }
        return area_of_union_of_rectangles(rectangles);
    };
    auto A = W * H;
    int m = times.size();
    vector<double> areas(m);
    for (int i = 0; i < m; i++) A = min(A, areas[i] = area(times[i]));

    for (int i = 0; i + 1 < m; i++) {
        auto tl = times[i], tr = times[i + 1], tm = tl + (tr - tl) / 2,
             a1 = areas[i], a2 = areas[i + 1], a3 = area(tm);

        A = min(A, a3);
        auto a = 2 * (a1 + a2 - 2 * a3), b = a2 - a1 - a, c = a1;
        for (auto r : quadratic_roots(0., 2 * a, b))
            if (fabsl(r.imag()) <= 1e-8) {
                auto x = r.real();
                if (0 <= x && x <= 1) A = min(A, a * x * x + b * x + c);
            }
    }
    cout << fixed << setprecision(15) << A;
}