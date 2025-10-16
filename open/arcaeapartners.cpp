#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    vector<T> BIT;
    function<T(T, T)> f;

    void update(int i, T v) {
        for (; i && i < BIT.size(); i += i & -i) BIT[i] = f(BIT[i], v);
    }

    T range_query(int i) {
        T v = 0;
        for (; i; i &= i - 1) v = f(v, BIT[i]);
        return v;
    }

    FenwickTree(int n, function<T(T, T)> func) : BIT(n, 0), f(move(func)) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> p(n);
    vector<array<int, 3>> values;
    for (int i = 0; i < n; i++) {
        int g, ga, pa;
        cin >> g >> p[i] >> ga >> pa;

        values.push_back({g, p[i], 0});
        if (ga && pa) {
            p.emplace_back(pa);
            values.push_back({ga, pa, 1});
        }
    }
    sort(p.begin(), p.end());
    p.erase(unique(p.begin(), p.end()), p.end());
    int count = p.size();

    for (auto &v : values) v[1] = lower_bound(p.begin(), p.end(), v[1]) - p.begin() + 1;
    sort(values.begin(), values.end(), [&](auto v1, auto v2) { return v1[0] != v2[0] ? v1[0] > v2[0] : v1[1] < v2[1]; });

    vector<FenwickTree<int>> fws(max(n, k) + 1, FenwickTree<int>(count + 1, [](int x, int y) { return max(x, y); }));
    for (auto [g, j, b] : values)
        if (!b)
            for (int i = 0; i <= k; i++) fws[i].update(j, fws[i].range_query(j) + 1);
        else
            for (int i = k; i; i--) fws[i].update(j, fws[i - 1].range_query(j) + 1);

    int d = 0;
    for (int i = 1; i <= n; i++) d = max(d, fws[i].range_query(count));
    cout << d;
}
