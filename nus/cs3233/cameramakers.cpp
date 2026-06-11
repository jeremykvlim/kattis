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

    const auto begin() const {
        return &x;
    }

    const auto end() const {
        return &y + 1;
    }

    Point operator-() const {
        return {-x, -y};
    }

    Point operator!() const {
        return {y, x};
    }

    Point operator~() const {
        return {-y, x};
    }

    bool operator<(const Point &p) const {
        return x != p.x ? x < p.x : y < p.y;
    }

    bool operator>(const Point &p) const {
        return x != p.x ? x > p.x : y > p.y;
    }

    bool operator==(const Point &p) const {
        return x == p.x && y == p.y;
    }

    bool operator!=(const Point &p) const {
        return x != p.x || y != p.y;
    }

    bool operator<=(const Point &p) const {
        return *this < p || *this == p;
    }

    bool operator>=(const Point &p) const {
        return *this > p || *this == p;
    }

    Point operator+(const Point &p) const {
        return {x + p.x, y + p.y};
    }

    Point operator+(const T &v) const {
        return {x + v, y + v};
    }

    Point & operator+=(const Point &p) {
        x += p.x;
        y += p.y;
        return *this;
    }

    Point & operator+=(const T &v) {
        x += v;
        y += v;
        return *this;
    }

    Point operator-(const Point &p) const {
        return {x - p.x, y - p.y};
    }

    Point operator-(const T &v) const {
        return {x - v, y - v};
    }

    Point & operator-=(const Point &p) {
        x -= p.x;
        y -= p.y;
        return *this;
    }

    Point & operator-=(const T &v) {
        x -= v;
        y -= v;
        return *this;
    }

    Point operator*(const T &v) const {
        return {x * v, y * v};
    }

    Point & operator*=(const T &v) {
        x *= v;
        y *= v;
        return *this;
    }

    Point operator/(const T &v) const {
        return {x / v, y / v};
    }

    Point & operator/=(const T &v) {
        x /= v;
        y /= v;
        return *this;
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
T squared_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

template <typename T>
struct KDTree {
    struct KDNode {
        int l, r;
        Point<T> p;
        T xl, xr, yl, yr;

        KDNode() {}
        KDNode(const Point<T> &p, T xl, T xr, T yl, T yr) : p(p), xl(xl), xr(xr), yl(yl), yr(yr) {}
    };

    vector<Point<T>> points;
    vector<KDNode> KDT;

    KDTree(int n, const vector<Point<T>> &p) : points(p), KDT(n) {
        int i = 0;
        build(i, 0, n);
    }

    int build(int &i, int l, int r) {
        if (l >= r) return -1;

        int j = i++;
        T xl = points[l].x, xr = points[l].x, yl = points[l].y, yr = points[l].y;
        for (int k = l + 1; k < r; k++) {
            xl = min(xl, points[k].x);
            xr = max(xr, points[k].x);
            yl = min(yl, points[k].y);
            yr = max(yr, points[k].y);
        }

        bool dir = (xr - xl) < (yr - yl);
        int m = l + (r - l) / 2;
        nth_element(points.begin() + l, points.begin() + m, points.begin() + r, [dir](const auto &a, const auto &b) { return !dir ? a < b : !a < !b; });

        KDT[j] = {points[m], xl, xr, yl, yr};
        KDT[j].l = build(i, l, m);
        KDT[j].r = build(i, m + 1, r);
        return j;
    }

    T kth_nearest_neighbor_dist(const Point<T> &p, int k) const {
        priority_queue<T> pq;
        kth_nearest_neighbor_dist(0, p, k, pq);
        return pq.top();
    }

    void kth_nearest_neighbor_dist(int i, const Point<T> &p, int k, priority_queue<T> &pq) const {
        if (!~i) return;

        T dist = squared_dist(p, KDT[i].p);
        if (pq.size() < k) pq.emplace(dist);
        else if (dist < pq.top()) {
            pq.pop();
            pq.emplace(dist);
        }

        int l = KDT[i].l, r = KDT[i].r;
        T dl = !~l ? numeric_limits<T>::max() : squared_dist(p, {min(KDT[l].xr, max(KDT[l].xl, p.x)), min(KDT[l].yr, max(KDT[l].yl, p.y))}),
          dr = !~r ? numeric_limits<T>::max() : squared_dist(p, {min(KDT[r].xr, max(KDT[r].xl, p.x)), min(KDT[r].yr, max(KDT[r].yl, p.y))});
        if (dl > dr) {
            swap(l, r);
            swap(dl, dr);
        }

        if (~l && (pq.size() < k || dl <= pq.top())) kth_nearest_neighbor_dist(l, p, k, pq);
        if (~r && (pq.size() < k || dr <= pq.top())) kth_nearest_neighbor_dist(r, p, k, pq);
    }
};

template <typename T>
double radius_of_k_enclosing_circle(int k, const vector<Point<T>> &points) {
    int n = points.size();
    T xl = numeric_limits<T>::max(), yl = numeric_limits<T>::max();
    for (auto [x, y] : points) {
        xl = min(xl, x);
        yl = min(yl, y);
    }

    auto R = 1e20;
    KDTree kdt(n, points);
    vector<T> k_dist(n);
    for (int i = 0; i < n; i++) {
        k_dist[i] = kdt.kth_nearest_neighbor_dist(points[i], k);
        R = min(R, sqrt(k_dist[i]));
    }

    vector<int> order(n);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j) { return k_dist[i] < k_dist[j]; });

    auto scale = 2 / R;
    auto row = [&](int i) -> int {
        return (points[i].y - yl) * scale;
    };

    auto col = [&](int i) -> int {
        return (points[i].x - xl) * scale;
    };
    vector<array<int, 3>> cell_points(n);
    for (int i = 0; i < n; i++) cell_points[i] = {col(i), row(i), i};
    sort(cell_points.begin(), cell_points.end());

    vector<array<int, 3>> rows, cols;
    vector<int> indices;
    for (auto [c, r, i] : cell_points) {
        if (cols.empty() || cols.back()[0] != c) cols.push_back({c, (int) rows.size(), (int) rows.size()});
        if (cols.back()[1] == rows.size() || rows.back()[0] != r) {
            rows.push_back({r, (int) indices.size(), (int) indices.size()});
            cols.back()[2] = rows.size();
        }
        indices.emplace_back(i);
        rows.back()[2] = indices.size();
    }

    for (int i : order) {
        if (k_dist[i] > 4 * R * R + 1e-7) break;

        vector<pair<double, double>> candidates;
        int c = col(i);
        for (auto it_col = lower_bound(cols.begin(), cols.end(), c - 4, [](auto &col, int c) { return col[0] < c; }); it_col != cols.end() && (*it_col)[0] <= c + 4; it_col++) {
            int r = row(i);
            auto it_row_l = lower_bound(rows.begin() + (*it_col)[1], rows.begin() + (*it_col)[2], r - 4, [](auto &row, int r) { return row[0] < r; });
            for (auto it_row = it_row_l; it_row != upper_bound(it_row_l, rows.begin() + (*it_col)[2], r + 4, [](int r, const auto &row) { return row[0] > r; }); it_row++)
                for (int p = (*it_row)[1]; p < (*it_row)[2]; p++) {
                    int j = indices[p];
                    if (i != j) {
                        auto d = euclidean_dist(points[i], points[j]);
                        if (d <= 2 * R) candidates.emplace_back(d, angle(points[i] - points[j]));
                    }
                }
        }
        sort(candidates.begin(), candidates.end());

        auto valid = [&](double radius) {
            int count = upper_bound(candidates.begin(), candidates.end(), 2 * radius, [&](double d, const pair<double, double> &p) { return d < p.first; }) - candidates.begin();
            if (count < k - 1) return false;
            if (count >= 7 * (k - 1)) return true;

            vector<pair<double, bool>> intervals;
            for (int i = 0; i < count; i++) {
                auto [d, a] = candidates[i];
                auto theta = acos(d / (2 * radius));
                intervals.emplace_back(a - theta, false);
                intervals.emplace_back(a + theta, true);
            }
            sort(intervals.begin(), intervals.end());

            int overlaps = 1;
            for (auto [angle, end] : intervals) {
                if (!end) overlaps++;
                else overlaps--;

                if (overlaps >= k) return true;
            }
            return false;
        };
        if (!valid(R)) continue;

        double l = sqrt(k_dist[i]) / 2, r = R, m;
        while (l + 1e-9 < r && l + l * 1e-9 < r) {
            m = l + (r - l) / 2;

            if (valid(m)) r = m;
            else l = m;
        }
        R = r;
    }
    return R;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<Point<long long>> points(n);
    for (auto &[x, y] : points) cin >> x >> y;
    cout << fixed << setprecision(2) << radius_of_k_enclosing_circle(k, points);
}
