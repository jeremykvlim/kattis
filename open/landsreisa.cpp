#include <bits/stdc++.h>
using namespace std;

template <typename T>
bool approximately_equal(const T &v1, const T &v2, double epsilon = 1e-5) {
    return fabs(v1 - v2) <= epsilon;
}

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

    bool operator<(const Point &p) const {
        return !approximately_equal(x, p.x) ? x < p.x : y < p.y;
    }

    bool operator>(const Point &p) const {
        return !approximately_equal(x, p.x) ? x > p.x : y > p.y;
    }

    bool operator==(const Point &p) const {
        if constexpr (is_floating_point_v<T>) return approximately_equal(x, p.x) && approximately_equal(y, p.y);
        return x == p.x && y == p.y;
    }

    bool operator!=(const Point &p) const {
        if constexpr (is_floating_point_v<T>) return !approximately_equal(x, p.x) || !approximately_equal(y, p.y);
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
double euclidean_dist(const Point<T> &a, const Point<T> &b = {0, 0}) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

template <typename F1, typename F2>
void simulated_annealing(const F1 &cost, const F2 &change, const double &temperature = 0.01, const double &cooling_rate = 0.999999, const int &time_limit = 1e3) {
    mt19937 rng(random_device{}());
    uniform_real_distribution<double> urd(0, 1);
    auto start = chrono::steady_clock::now();
    for (auto t = temperature; chrono::steady_clock::now() - start < chrono::milliseconds(time_limit); t *= cooling_rate) {
        auto delta = cost();
        if (delta < 0 || exp(-delta / t) > urd(rng)) change();
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Point<double>> points(n);
    for (auto &[x, y] : points) cin >> x >> y;

    vector<vector<double>> dist(n, vector<double>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) dist[i][j] = euclidean_dist(points[i], points[j]);

    vector<int> tour(n, 0);
    vector<bool> visited(n, false);
    visited[0] = true;
    for (int i = 1, v = 0; i < n; i++) {
        int u = -1;
        auto d = 1e9;
        for (int j = 0; j < n; j++)
            if (!visited[j] && dist[v][j] < d) {
                d = dist[v][j];
                u = j;
            }

        tour[i] = u;
        visited[u] = true;
        v = u;
    }

    auto length = [&](const vector<int> &tour) -> double {
        double len = dist[tour[n - 1]][tour[0]];
        for (int i = 0; i < n - 1; i++) len += dist[tour[i]][tour[i + 1]];
        return len;
    };

    auto two_opt = [&](vector<int> &tour) {
        bool change;
        do {
            change = false;
            for (int i = 1; i < n - 1; i++)
                for (int j = i + 1; j < n; j++) {
                    int a = tour[i - 1], b = tour[i], c = tour[j], d = (j + 1 == n ? tour[0] : tour[j + 1]);
                    if (dist[a][c] + dist[b][d] < dist[a][b] + dist[c][d] - 1e-9) {
                        reverse(tour.begin() + i, tour.begin() + j + 1);
                        change = true;
                    }
                }
        } while (change);
    };
    two_opt(tour);

    mt19937 rng(random_device{}());
    auto sample = [&]() -> pair<int, int> {
        int i = -1, j = -1;
        while (i == j) {
            i = rng() % (n - 1) + 1;
            j = rng() % (n - 1) + 1;
        }
        return minmax(i, j);
    };

    int i = 0, j = 0;
    simulated_annealing([&]() {
        tie(i, j) = sample();
        int a = tour[i - 1], b = tour[i], c = tour[j], d = (j + 1 == n ? tour[0] : tour[j + 1]);
        return dist[a][c] + dist[b][d] - dist[a][b] - dist[c][d];
    }, [&]() {
        reverse(tour.begin() + i, tour.begin() + j + 1);
    });

    auto len = length(tour);
    while (len > 63) {
        auto t = tour;
        tie(i, j) = sample();
        reverse(t.begin() + i, t.begin() + j + 1);
        two_opt(t);

        auto l = length(t);
        if (len > l) {
            len = l;
            tour = t;
        }
    }
    for (int v : tour) cout << v << " ";
}
