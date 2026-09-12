#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    template <typename U>
    Point(U x, U y) : x(x), y(y) {}

    template <typename U>
    Point(const Point<U> &p) : x((T) p.x), y((T) p.y) {}

    Point operator~() const {
        return {-y, x};
    }

    bool operator<(const Point &p) const {
        return x != p.x ? x < p.x : y < p.y;
    }

    bool operator==(const Point &p) const {
        return x == p.x && y == p.y;
    }

    Point operator+(const Point &p) const {
        return {x + p.x, y + p.y};
    }

    Point operator-(const Point &p) const {
        return {x - p.x, y - p.y};
    }

    Point operator*(const T &v) const {
        return {x * v, y * v};
    }

    Point operator/(const T &v) const {
        return {x / v, y / v};
    }
};

template <typename T>
double angle(const Point<T> &p) {
    return atan2(p.y, p.x);
}

template <typename T>
double euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

template <typename T>
struct Circle {
    Point<T> origin;
    T radius;

    Circle() {}
    Circle(const Point<T> &o, const T &r) : origin(o), radius(r) {}

    bool operator==(const Circle &c) const {
        return origin == c.origin && radius == c.radius;
    }
};

template <typename T>
vector<Point<T>> circle_circle_intersections(const Circle<T> &c1, const Circle<T> &c2) {
    if (c1 == c2) return {};

    T d = euclidean_dist(c1.origin, c2.origin);
    if (d > c1.radius + c2.radius || d < abs(c1.radius - c2.radius)) return {};

    T r1_sq = c1.radius * c1.radius, r2_sq = c2.radius * c2.radius,
      l = (d * d + r1_sq - r2_sq) / (2 * d);

    auto v = c2.origin - c1.origin, p = c1.origin + v * l / d;
    if (d == c1.radius + c2.radius) return {p};

    T h = sqrt(r1_sq - l * l);
    auto u = ~v, q = u * h / d;
    return {p + q, p - q};
}

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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<Circle<double>> circles(n);
    for (auto &c : circles) cin >> c.origin.x >> c.origin.y >> c.radius;

    vector<bool> removed(n, false);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (i != j)
                if (euclidean_dist(circles[i].origin, circles[j].origin) <= circles[j].radius - circles[i].radius || circles[i] == circles[j] && j < i) {
                    removed[i] = true;
                    break;
                }

    vector<Circle<double>> temp;
    for (int i = 0; i < n; i++)
        if (!removed[i]) temp.emplace_back(circles[i]);
    circles = temp;

    vector<vector<pair<double, double>>> open(circles.size());
    for (int i = 0; i < circles.size(); i++) {
        vector<pair<double, double>> closed;
        for (int j = 0; j < circles.size(); j++)
            if (i != j) {
                auto d = euclidean_dist(circles[i].origin, circles[j].origin);
                if (abs(circles[i].radius - circles[j].radius) < d && d < circles[i].radius + circles[j].radius) {
                    auto points = circle_circle_intersections(circles[i], circles[j]);
                    auto l = angle(points[1] - circles[i].origin), r = angle(points[0] - circles[i].origin);
                    if (l < r) closed.emplace_back(l, r);
                    else {
                        closed.emplace_back(-M_PI, r);
                        closed.emplace_back(l, M_PI);
                    }
                }
            }
        sort(closed.begin(), closed.end());

        auto a = -M_PI;
        for (auto [l, r] : closed) {
            if (a < l) open[i].emplace_back(a, l);
            a = max(a, r);
        }
        if (a < M_PI) open[i].emplace_back(a, M_PI);
    }

    while (q--) {
        double x, y, w, h;
        cin >> x >> y >> w >> h;

        vector<array<double, 4>> rectangles;
        for (auto &[o, r] : circles) {
            auto dy = y + h - o.y;
            if (abs(dy) >= r) continue;

            auto dx = sqrt(r * r - dy * dy), xl = max(x, o.x - dx), xr = min(x + w, o.x + dx);
            if (xl < xr) rectangles.push_back({xl, xr, y, y + h});
        }

        auto area = area_of_union_of_rectangles(rectangles);
        for (int i = 0; i < circles.size(); i++) {
            auto &[o, R] = circles[i];
            if (x - R < o.x && o.x < x + w + R && y - R < o.y && o.y < y + h + R) {
                int outside = 0;
                vector<pair<double, int>> sweep;
                if (x + R > o.x) {
                    auto a = acos((x - o.x) / R);
                    sweep.emplace_back(-a, -1);
                    sweep.emplace_back(a, 1);
                    outside++;
                }

                if (x + w - R < o.x) {
                    auto a = acos((x + w - o.x) / R);
                    sweep.emplace_back(-a, 1);
                    sweep.emplace_back(a, -1);
                }

                if (y + R > o.y) {
                    auto a = asin((y - o.y) / R);
                    if (a > 0) {
                        sweep.emplace_back(M_PI - a, 1);
                        outside++;
                    } else sweep.emplace_back(-M_PI - a, 1);

                    sweep.emplace_back(a, -1);
                }

                if (y + h - R < o.y) {
                    auto a = asin((y + h - o.y) / R);
                    if (a < 0) {
                        sweep.emplace_back(-M_PI - a, -1);
                        outside++;
                    } else sweep.emplace_back(M_PI - a, -1);

                    sweep.emplace_back(a, 1);
                }
                sort(sweep.begin(), sweep.end());
                sweep.emplace_back(M_PI + 1, 0);

                for (int j = 0; auto [l, r] : open[i]) {
                    for (; sweep[j].first < l; j++) outside += sweep[j].second;

                    for (auto a = l;; j++) {
                        auto b = min(r, sweep[j].first);
                        if (!outside && a < b) area -= R * (o.y - y) * (cos(b) - cos(a)) + 0.5 * R * R * (sin(b) * cos(b) - sin(a) * cos(a) - b + a);
                        if (sweep[j].first >= r) break;

                        a = sweep[j].first;
                        outside += sweep[j].second;
                    }
                }
            }
        }
        cout << fixed << setprecision(5) << 100 * area / (w * h) << "\n";
    }
}