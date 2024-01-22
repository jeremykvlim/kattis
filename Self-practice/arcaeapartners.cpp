#include <bits/stdc++.h>
using namespace std;

void update(vector<vector<int>> &fenwick, int i, int j, int value) {
    for (; i < fenwick[j].size(); i += i & -i) fenwick[j][i] = max(fenwick[j][i], value);
}


int query(vector<vector<int>> &fenwick, int i, int j) {
    int value = 0;
    for (; i; i ^= i & -i) value = max(value, fenwick[j][i]);
    return value;
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<vector<int>> values;
    map<int, int> steps;
    for (int i = 0; i < n; i++) {
        int g, p, ga, pa;
        cin >> g >> p >> ga >> pa;
        
        steps[p];
        values.push_back({g, p, 0});
        if (ga && pa) {
            steps[pa];
            values.push_back({ga, pa, 1});
        }
    }

    int count = 0;
    for (auto &p : steps) p.second = ++count;
    for (auto &v : values) v[1] = steps[v[1]];
    sort(values.begin(), values.end(), [&](auto v1, auto v2) {return v1[0] != v2[0] ? v1[0] > v2[0] : v1[1] < v2[1];});

    vector<vector<int>> fenwick(max(n, k) + 1, vector<int>(count + 1, 0));
    for (auto v : values)
        if (!v[2])
            for (int i = 0; i <= k; i++) update(fenwick, v[1], i, query(fenwick, v[1], i) + 1);
        else
            for (int i = k; i; i--) update(fenwick, v[1], i, query(fenwick, v[1], i - 1) + 1);

    int diversity = 0;
    for (int i = 1; i <= n; i++) diversity = max(diversity, query(fenwick, count, i));
    cout << diversity;
}
