#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    auto operator==(const Point &p) const {
        return x == p.x && y == p.y;
    }

    Point operator+(Point p) const {
        return {x + p.x, y + p.y};
    }

    auto operator+=(Point p) {
        x += p.x;
        y += p.y;
    }
};

template <typename T>
T manhattan_dist(Point<T> a, Point<T> b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

struct Hash {
    size_t operator()(Point<int> p) const {
        auto h = hash<int>()(p.x);
        h ^= hash<int>()(p.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
        return h;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Point<int>> points(n);
    for (auto &[x, y] : points) cin >> x >> y;

    string moves;
    auto move = [&](bool flip) {
        Point<int> curr{0, 0};
        int curr_k = 0;
        unordered_set<Point<int>, Hash> visited;
        visited.emplace(curr);

        vector<int> dx{1, 0, -1, 0}, dy{0, 1, 0, -1};
        auto step = [&](int k) {
            if (!k) moves += ">^"[flip];
            else if (k == 1) moves += "^>"[flip];
            else if (k == 2) moves += "<v"[flip];
            else moves += "v<"[flip];

            curr_k = k;
            curr += {dx[k], dy[k]};
            visited.emplace(curr);
        };

        for (auto P : points) {
            if (flip) swap(P.x, P.y);

            vector<Point<int>> border;
            auto start = curr + Point(dx[curr_k], dy[curr_k]);
            int k = (curr_k + 3) % 4;
            for (auto p = start;; k = (k + 3) % 4) {
                border.emplace_back(p);
                for (int i = 0; visited.count(p + Point(dx[k], dy[k])); i++) {
                    if (i > 4) {
                        moves.clear();
                        return false;
                    }
                    k = (k + 1) % 4;
                }

                p += {dx[k], dy[k]};
                if (p == start) break;
            }

            Point<int> p{(int) 1e9, (int) 1e9};
            for (auto pt : border)
                if (manhattan_dist(P, p) > manhattan_dist(P, pt)) p = pt;

            int j = find(border.begin(), border.end(), p) - border.begin();
            if (j > border.size() - j) {
                reverse(border.begin() + 1, border.end());
                j = find(border.begin(), border.end(), p) - border.begin();
            }

            step(curr_k);
            for (int i = 0; i < j; i++) {
                auto [x1, y1] = border[i];
                auto [x2, y2] = border[i + 1];

                if (x2 > x1) step(0);
                else if (y2 > y1) step(1);
                else if (x2 < x1) step(2);
                else step(3);
            }

            while (curr.x > P.x) step(2);
            while (curr.x < P.x) step(0);
            while (curr.y > P.y) step(3);
            while (curr.y < P.y) step(1);
        }
        return true;
    };
    if (!move(false)) move(true);
    cout << moves;
}