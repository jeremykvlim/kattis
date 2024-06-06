#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree2D {
    vector<vector<T>> BIT;

    void update(int i, int j, T value) {
        for (; i < BIT[j].size(); i += i & -i) BIT[j][i] = max(BIT[j][i], value);
    }

    T rmq(int i, int j) {
        T value = 0;
        for (; i; i &= (i - 1)) value = max(value, BIT[j][i]);
        return value;
    }

    FenwickTree2D(int n, int m) : BIT(n, vector<T>(m, 0)) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<tuple<int, int, int>> values;
    map<int, int> step;
    for (int i = 0; i < n; i++) {
        int g, p, ga, pa;
        cin >> g >> p >> ga >> pa;

        step[p];
        values.emplace_back(g, p, 0);
        if (ga && pa) {
            step[pa];
            values.emplace_back(ga, pa, 1);
        }
    }

    int index = 1;
    for (auto &[p, i] : step) i = index++;
    for (auto &[g, p, b] : values) p = step[p];
    sort(values.begin(), values.end(), [&](auto v1, auto v2) {return get<0>(v1) != get<0>(v2) ? get<0>(v1) > get<0>(v2) : get<1>(v1) < get<1>(v2);});

    FenwickTree2D<int> fw(max(n, k) + 1, index + 1);
    for (auto [g, p, b] : values)
        if (!b)
            for (int i = 0; i <= k; i++) fw.update(p, i, fw.rmq(p, i) + 1);
        else
            for (int i = k; i; i--) fw.update(p, i, fw.rmq(p, i - 1) + 1);

    int d = 0;
    for (int i = 1; i <= n; i++) d = max(d, fw.rmq(index, i));
    cout << d;
}
