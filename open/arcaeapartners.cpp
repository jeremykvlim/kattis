#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree2D {
    vector<vector<T>> BIT;
    function<T(T, T)> f;

    FenwickTree2D(int n, int m, function<T(T, T)> func) : BIT(n, vector<T>(m, 0)), f(move(func)) {}

    void update(int i, int j, T v) {
        for (; i && i < BIT[j].size(); i += i & -i) BIT[j][i] = f(BIT[j][i], v);
    }

    T range_query(int i, int j) {
        T v = 0;
        for (; i; i &= i - 1) v = f(v, BIT[j][i]);
        return v;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<array<int, 3>> values;
    map<int, int> indices;
    for (int i = 0; i < n; i++) {
        int g, p, ga, pa;
        cin >> g >> p >> ga >> pa;

        indices[p];
        values.push_back({g, p, 0});
        if (ga && pa) {
            indices[pa];
            values.push_back({ga, pa, 1});
        }
    }

    int index = 1;
    for (auto &[p, i] : indices) i = index++;
    for (auto &[g, p, b] : values) p = indices[p];
    sort(values.begin(), values.end(), [&](auto v1, auto v2) { return v1[0] != v2[0] ? v1[0] > v2[0] : v1[1] < v2[1]; });

    FenwickTree2D<int> fw(max(n, k) + 1, index + 1, [](int x, int y) { return max(x, y); });
    for (auto [g, p, b] : values)
        if (!b)
            for (int i = 0; i <= k; i++) fw.update(p, i, fw.range_query(p, i) + 1);
        else
            for (int i = k; i; i--) fw.update(p, i, fw.range_query(p, i - 1) + 1);

    int d = 0;
    for (int i = 1; i <= n; i++) d = max(d, fw.range_query(index, i));
    cout << d;
}
