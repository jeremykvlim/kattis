#include <bits/stdc++.h>
using namespace std;

template <typename T, typename F>
struct FenwickTree2D {
    vector<vector<T>> BIT;
    F f;

    void update(int i, int j, T value) {
        for (; i < BIT[j].size(); i += i & -i) BIT[j][i] = f(BIT[j][i], value);
    }

    T range_query(int i, int j) {
        T value = 0;
        for (; i; i &= (i - 1)) value = f(value, BIT[j][i]);
        return value;
    }

    FenwickTree2D(int n, int m, F func) : BIT(n, vector<T>(m, 0)), f(func) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<tuple<int, int, int>> values;
    map<int, int> indices;
    for (int i = 0; i < n; i++) {
        int g, p, ga, pa;
        cin >> g >> p >> ga >> pa;

        indices[p];
        values.emplace_back(g, p, 0);
        if (ga && pa) {
            indices[pa];
            values.emplace_back(ga, pa, 1);
        }
    }

    int index = 1;
    for (auto &[p, i] : indices) i = index++;
    for (auto &[g, p, b] : values) p = indices[p];
    sort(values.begin(), values.end(), [&](auto v1, auto v2) {return get<0>(v1) != get<0>(v2) ? get<0>(v1) > get<0>(v2) : get<1>(v1) < get<1>(v2);});

    auto _max = [](int x, int y) {return max(x, y);};
    FenwickTree2D<int, decltype(_max)> fw(max(n, k) + 1, index + 1, _max);
    for (auto [g, p, b] : values)
        if (!b)
            for (int i = 0; i <= k; i++) fw.update(p, i, fw.range_query(p, i) + 1);
        else
            for (int i = k; i; i--) fw.update(p, i, fw.range_query(p, i - 1) + 1);

    int d = 0;
    for (int i = 1; i <= n; i++) d = max(d, fw.range_query(index, i));
    cout << d;
}
