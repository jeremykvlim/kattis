#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    auto operator<(Point &p) const {
        return x != p.x ? x < p.x : y < p.y;
    }

    auto operator!=(const Point &p) const {
        return x != p.x || y != p.y;
    }

    Point operator-(Point p) const {
        return {x - p.x, y - p.y};
    }
};

template <typename T>
double dist(Point<T> p) {
    return sqrt(p.x * p.x + p.y * p.y);
}

template <typename T>
T cross(Point<T> a, Point<T> b, Point<T> c) {
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

template <typename T>
void add(deque<pair<Point<T>, int>> &half_hull, vector<vector<int>> &adj_list, pair<Point<T>, int> p) {
    while (half_hull.size() > 1 && cross(half_hull[1].first, half_hull[0].first, p.first) > 0) half_hull.pop_front();
    adj_list[half_hull[0].second].emplace_back(p.second);
    half_hull.emplace_front(p);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    double d;
    cin >> n >> d;

    bool collide = false;
    vector<Point<long long>> coords(n + 1);
    coords[0] = {0, 0};
    for (int i = 1; i <= n; i++) {
        cin >> coords[i].x >> coords[i].y;

        if (dist(coords[i]) <= d) collide = true;
    }

    if (!collide) {
        cout << -1;
        exit(0);
    }

    vector<int> ord{0};
    for (int i = 0, count = 0, quadrant = 0;; ++quadrant %= 4) {
        vector<pair<Point<long long>, int>> points;
        for (int j = 1; j <= n; j++) points.emplace_back(coords[j] - coords[i], j);

        auto rotate = [&](auto &p) {
            if (quadrant == 1) p = {p.y, -p.x};
            else if (quadrant == 2) p = {-p.x, -p.y};
            else p = {-p.y, p.x};
        };
        if (quadrant)
            for (auto &[p, _] : points) rotate(p);

        auto temp = points;
        for (int j = 0; j < temp.size(); j++) temp[j].second = j;

        temp.erase(remove_if(temp.begin(), temp.end(), [&](auto p) {return p.first.x < 0 || p.first.y < 0;}), temp.end());
        if (temp.empty()) {
            next:;
            if (++count == 4) {
                cout << i;
                exit(0);
            }

            continue;
        }

        vector<int> indices;
        if (!temp.empty()) {
            sort(temp.begin(), temp.end(), [&](auto p1, auto p2) {return p1.first < p2.first;});
            if (temp.front().first != Point<long long>{0, 0}) temp.insert(temp.begin(), {Point<long long>{0, 0}, 0});

            deque<pair<Point<long long>, int>> half_hull{{Point<long long>{0, 0}, 0}};
            vector<vector<int>> adj_list(temp.size());
            for (int j = 1; j < temp.size(); j++) add(half_hull, adj_list, {temp[j].first, j});

            int v = 0;
            for (auto len = d; len >= 0;) {
                auto it = find_if(adj_list[v].rbegin(), adj_list[v].rend(), [&](int u) {return len >= dist(temp[v].first - temp[u].first);});
                if (it == adj_list[v].rend()) break;

                int u = *it;
                len -= dist(temp[v].first - temp[u].first);
                v = u;
                indices.emplace_back(temp[u].second);
            }
            if (indices.empty()) goto next;
        }

        count = 0;
        d -= dist(points[indices[0]].first);
        for (int j = 1; j < indices.size(); j++) d -= dist(points[indices[j]].first - points[indices[j - 1]].first);
        i = points[indices.back()].second;

        for (int j : indices) ord.emplace_back(points[j].second);
        if (ord.size() >= 3)
            for (int k = ord.size() - 3; ~k; k--)
                if (ord[k] == ord[ord.size() - 2] && ord[k + 1] == ord.back()) {
                    double len = 0;
                    for (int j = k + 1; j < ord.size() - 1; j++) len += dist(coords[ord[j + 1]] - coords[ord[j]]);

                    if (d > len) d -= (int) (d / len - 1) * len;
                    break;
                }

    }
}
