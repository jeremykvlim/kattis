#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    auto operator<(const Point &p) const {
        return x != p.x ? x < p.x : y < p.y;
    }

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

    auto operator/=(T c) {
        x /= c;
        y /= c;
    }
};

template <typename T>
double squared_dist(Point<T> p) {
    return p.x * p.x + p.y * p.y;
}

template <typename T>
double squared_dist(Point<T> a, Point<T> b) {
    return (double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    Point<int> cat(0, 0);
    vector<Point<int>> curr, prev;
    vector<int> dx{1, 0, -1, 0, 2, 0, -2, 0, 1, -1, 1, -1}, dy{0, 1, 0, -1, 0, 2, 0, -2, 1, 1, -1, -1};
    vector<string> dir{"R", "U", "L", "D", "RR", "UU", "LL", "DD", "RU", "LU", "RD", "LD"};
    mt19937 rng(random_device{}());
    for (;;) {
        int i = 0;
        if (!prev.empty()) {
            sort(prev.begin(), prev.end());
            prev.erase(unique(prev.begin(), prev.end()), prev.end());
            
            vector<int> possible;
            for (int k = 0; k < 12; k++)
                if (find(prev.begin(), prev.end(), cat + Point(dx[k], dy[k])) != prev.end()) possible.emplace_back(k);

            if (!possible.empty()) i = possible[rng() % possible.size()];
            else if (!(rng() % 3)) i = rng() % 8 + 4;
            else {
                Point<int> mouse_ave(0, 0);
                for (auto pos: prev) mouse_ave += pos;
                mouse_ave /= prev.size();

                double dist = 1e20;
                for (int k = 0; k < 12; k++) {
                    auto d = squared_dist(cat + Point(dx[k], dy[k]), mouse_ave);
                    if (dist > d) {
                        dist = d;
                        i = k;
                    }
                }
            }
        }
        cout << dir[i].size() << " " << dir[i] << "\n" << flush;

        int d;
        cin >> d;

        if (!d) exit(0);

        cat += Point(dx[i], dy[i]);
        if (curr.empty()) {
            for (int j = 0; j * j <= d; j++) {
                Point<int> gap(j, ceil(sqrt(d - j * j)));
                if (squared_dist(gap) == d) {
                    curr.emplace_back(cat + gap);
                    if (gap.x) curr.emplace_back(cat.x - gap.x, cat.y + gap.y);
                    if (gap.y) curr.emplace_back(cat.x + gap.x, cat.y - gap.y);
                    if (gap.x && gap.y) curr.emplace_back(cat.x - gap.x, cat.y - gap.y);
                }
            }
        } else {
            curr.clear();
            for (auto pos : prev)
                for (int k = 0; k < 4; k++)
                    if (squared_dist(cat, pos + Point(dx[k], dy[k])) == d) curr.emplace_back(pos + Point(dx[k], dy[k]));
        }
        swap(curr, prev);
    }
}
