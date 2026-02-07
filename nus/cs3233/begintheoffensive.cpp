#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long X;
    cin >> n >> X;

    vector<long long> x(n);
    for (auto &xi : x) {
        cin >> xi;

        if (xi < X) {
            cout << -1;
            exit(0);
        }
    }

    DisjointSets dsu(n + 1);
    vector<int> range_end(n, n - 1);
    vector<pair<long long, int>> dp, temp;
    for (int r = 0; r < n; r++) {
        temp.clear();

        for (auto [v, l] : dp)
            if (temp.empty() || temp.back().first != (v & x[r])) temp.emplace_back(v & x[r], l);
        if (temp.empty() || temp.back().first != x[r]) temp.emplace_back(x[r], r);

        for (int i = 0; i < temp.size(); i++) {
            auto v = temp[i].first;
            if (v >= X) continue;

            for (int l = dsu.find(temp[i].second); l <= (i + 1 < temp.size() ? temp[i + 1].second - 1 : r); l = dsu.find(l)) {
                range_end[l] = r - 1;
                dsu.unite(l + 1, l);
            }
        }
        dp = temp;
    }

    for (int r = range_end[0] + 1; r; r--) {
        for (int l = 0; l < n; l += r)
            if (range_end[l] < min(n - 1, l + r - 1)) goto next;
        cout << r;
        exit(0);
        next:;
    }

    cout << -1;
}
