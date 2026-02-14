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
T dot(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.x) + (a.y * b.y);
}

template <typename T>
T cross(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.y) - (a.y * b.x);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    vector<Point<int>> pos(10);
    for (int i = 1; i <= 9; i++) pos[i] = {(i - 1) % 3, 2 - (i - 1) / 3};

    vector<vector<int>> between(10, vector<int>(10, 0));
    between[1][3] = between[3][1] = 2;
    between[1][7] = between[7][1] = 4;
    between[1][9] = between[2][8] = between[3][7] = between[4][6] = between[6][4] = between[7][3] = between[8][2] = between[9][1] = 5;
    between[3][9] = between[9][3] = 6;
    between[7][9] = between[9][7] = 8;

    vector<int> p(9);
    iota(p.begin(), p.end(), 1);
    int patterns = 0;
    do {
        vector<bool> visited(10, false);
        visited[p[0]] = true;
        for (int i = 0; i < 8; i++) {
            if (between[p[i]][p[i + 1]] && !visited[between[p[i]][p[i + 1]]]) goto next;
            visited[p[i + 1]] = true;
        }

        for (int i = 1; i <= 7; i++) {
            auto v = pos[p[i]] - pos[p[i - 1]], u = pos[p[i + 1]] - pos[p[i]];
            int cp = cross(v, u);
            char c;
            if (!cp) c = dot(v, u) > 0 ? 'S' : 'A';
            else c = cp > 0 ? 'L' : 'R';

            if (s[i - 1] != '?' && s[i - 1] != c) goto next;
        }
        patterns++;
        next:;
    } while (next_permutation(p.begin(), p.end()));
    cout << patterns;
}
