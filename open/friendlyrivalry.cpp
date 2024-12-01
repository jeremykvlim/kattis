#include <bits/stdc++.h>
using namespace std;

struct DisjointSet {
    vector<int> sets;

    int find(int p) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p]));
    }

    bool unite(int p, int q) {
        int p_set = find(p), q_set = find(q);
        if (p_set != q_set) {
            sets[q_set] = p_set;
            return true;
        }
        return false;
    }

    DisjointSet(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

template <typename T>
struct Point {
    T x, y;

    Point() {}
    Point(T x, T y) : x(x), y(y) {}
};

template <typename T>
double dist(Point<T> a, Point<T> b) {
    return sqrt((double) (a.x - b.x) * (a.x - b.x) + (double) (a.y - b.y) * (a.y - b.y));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int c = 2 * n;
    vector<Point<long long>> coords(c);
    for (auto &[x, y] : coords) cin >> x >> y;

    vector<double> d;
    for (int i = 0; i < c; i++)
        for (int j = i + 1; j < c; j++) d.emplace_back(dist(coords[i], coords[j]));
    sort(d.begin(), d.end());
    d.erase(unique(d.begin(), d.end(), [&](double d1, double d2) {return abs(d1 - d2) < 1e-9;}), d.end());

    double distance = 0;
    vector<int> blue;
    int l = -1, r = d.size(), mid;
    while (l + 1 < r) {
        mid = l + (r - l) / 2;

        DisjointSet dsu(c);
        for (int i = 0; i < c; i++)
            for (int j = i + 1; j < c; j++)
                if (dist(coords[i], coords[j]) + 1e-9 < d[mid]) dsu.unite(i, j);

        vector<vector<int>> components(c);
        for (int i = 0; i < c; i++) components[dsu.find(i)].emplace_back(i);

        vector<int> prev(n + 1);
        vector<bool> visited1(n + 1, false);
        visited1[0] = true;
        for (int i = 0; i < c; i++)
            for (int j = n, s = components[i].size(); j >= s; j--)
                if (visited1[j - s] && !visited1[j]) {
                    prev[j] = i;
                    visited1[j] = true;
                }

        if (!visited1[n]) r = mid;
        else {
            distance = d[mid];

            vector<bool> visited2(c, false);
            int temp = n;
            while (temp) {
                visited2[prev[temp]] = true;
                temp -= components[prev[temp]].size();
            }

            vector<int> b;
            for (int i = 0; i < c; i++)
                if (visited2[i])
                    for (int chapter : components[i]) b.emplace_back(chapter);

            blue = b;
            l = mid;
        }
    }

    cout << fixed << setprecision(6) << distance << "\n";
    for (int chapter : blue) cout << chapter + 1 << "\n";
}
