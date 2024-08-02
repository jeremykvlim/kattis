#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}
};

template <typename T>
double dist(Point<T> a, Point<T> b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, d;
    cin >> n >> d;

    vector<Point<int>> points(n);
    vector<bitset<100>> valid(n, bitset<100>(0));
    for (int i = 0; i < n; i++) {
        cin >> points[i].x >> points[i].y;

        for (int j = i; ~j; j--)
            if (dist(points[i], points[j]) <= d) valid[i][j] = valid[j][i] = true;
    }

    vector<int> indices(n);
    iota(indices.begin(), indices.end(), 0);
    bitset<100> sensors(0);
    while (n--) {
        shuffle(indices.begin(), indices.end(), mt19937(random_device()()));

        auto temp = ~bitset<100>();
        for (int i : indices)
            if (temp[i]) temp &= valid[i];

        if (sensors.count() < temp.count()) sensors = temp;
    }

    cout << sensors.count() << "\n";
    for (int i = 0; i < 100; i++)
        if (sensors[i]) cout << i + 1 << " ";
}
