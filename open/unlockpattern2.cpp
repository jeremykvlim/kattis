#include <bits/stdc++.h>
using namespace std;

template <typename T>
bool approximately_equal(const T &v1, const T &v2) {
    return fabs(v1 - v2) <= 1e-6;
}

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}

    template <typename U>
    Point(const Point<U> &p) : x((T) p.x), y((T) p.y) {}

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

    struct Hash {
        size_t operator()(Point<T> p) const {
            auto h = 0ULL;
            h ^= hash<T>()(p.x) + 0x9e3779b9 + (h << 6) + (h >> 2);
            h ^= hash<T>()(p.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
            return h;
        }
    };
};

template <typename T>
T cross(const Point<T> &a, const Point<T> &b) {
    return (a.x * b.y) - (a.y * b.x);
}

bool valid(int p1, int p2, vector<bool> visited, vector<vector<int>> between) {
    if (p1 > p2) swap(p1, p2);
    return !between[p1][p2] || visited[between[p1][p2]];
}

void unlock(int curr, int prev, vector<bool> visited, vector<vector<int>> &between, vector<Point<int>> &pos, vector<string> &patterns, string p = "") {
    if (all_of(visited.begin(), visited.end(), [&](auto b) {return b;})) {
        patterns.emplace_back(p);
        return;
    }

    Point<int> prev_dir = pos[curr] - pos[prev], next_dir;
    for (int next = 1; next <= 9; next++)
        if (!visited[next] && valid(curr, next, visited, between)) {
            next_dir = pos[next] - pos[curr];

            int cross_product = cross(prev_dir, next_dir);
            p += (cross_product < 0) ? 'R' : (cross_product > 0) ? 'L' : (prev_dir == next_dir) ? 'S' : 'A';

            visited[next] = true;
            unlock(next, curr, visited, between, pos, patterns, p);
            visited[next] = false;
            p.pop_back();
        }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    vector<vector<int>> between(10, vector<int>(10, 0));
    between[1][3] = 2;
    between[1][7] = 4;
    between[1][9] = between[2][8] = between[3][7] = between[4][6] = 5;
    between[3][9] = 6;
    between[7][9] = 8;

    vector<Point<int>> pos(10);
    for (int i = 1; i <= 9; i++) pos[i] = {1 + (i - 1) / 3, 1 + (i - 1) % 3};

    vector<string> patterns;
    for (int i = 1; i <= 9; i++)
        for (int j = 1; j <= 9; j++) {
            vector<bool> visited(10, false);
            visited[0] = true;
            if (i != j && valid(i, j, visited, between)) {
                visited[i] = visited[j] = true;
                unlock(i, j, visited, between, pos, patterns);
            }
        }

    cout << count_if(patterns.begin(), patterns.end(), [&](auto p) {return equal(s.begin(), s.end(), p.begin(), [](char c1, char s1) {return c1 == '?' || c1 == s1;});});
}
