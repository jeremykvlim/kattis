#include <bits/stdc++.h>
using namespace std;

void update(vector<vector<int>> &fenwick, int i, int j, int value) {
    for (; i < fenwick[j].size(); i += i & -i) fenwick[j][i] = max(fenwick[j][i], value);
}

int query(vector<vector<int>> &fenwick, int i, int j) {
    int value = 0;
    for (; i; i &= (i - 1)) value = max(value, fenwick[j][i]);
    return value;
}

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

    int index = 0;
    for (auto &[p, i] : step) i = ++index;
    for (auto &[g, p, b] : values) p = step[p];
    sort(values.begin(), values.end(), [&](auto v1, auto v2) {return get<0>(v1) != get<0>(v2) ? get<0>(v1) > get<0>(v2) : get<1>(v1) < get<1>(v2);});

    vector<vector<int>> fenwick(max(n, k) + 1, vector<int>(index + 1, 0));
    for (auto [g, p, b] : values)
        if (!b)
            for (int i = 0; i <= k; i++) update(fenwick, p, i, query(fenwick, p, i) + 1);
        else
            for (int i = k; i; i--) update(fenwick, p, i, query(fenwick, p, i - 1) + 1);

    int d = 0;
    for (int i = 1; i <= n; i++) d = max(d, query(fenwick, index, i));
    cout << d;
}
