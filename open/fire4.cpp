#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct JumpTable {
    int lg;
    vector<vector<int>> lift;
    vector<vector<T>> sum;

    JumpTable(int n, int m, const vector<int> &next, const vector<T> &w) {
        lg = __lg(n) + 1;
        lift.assign(lg, vector<int>(m));
        lift[0] = next;
        sum.assign(lg, vector<T>(m));
        sum[0] = w;

        for (int b = 1; b < lg; b++)
            for (int i = 0; i < m; i++) {
                int j = lift[b - 1][i];
                lift[b][i] = lift[b - 1][j];
                sum[b][i] = sum[b - 1][i] + sum[b - 1][j];
            }
    }

    pair<T, int> jump_up(int v, int k) const {
        T cost = 0;
        for (int b = 0; b < lg; b++)
            if ((k >> b) & 1) {
                cost += sum[b][v];
                v = lift[b][v];
            }
        return {cost, v};
    }

    pair<T, int> jump_down(int v, T bound) const {
        T cost = 0;
        for (int b = lg - 1; ~b; b--)
            if (lift[b][v] <= bound) {
                cost += sum[b][v];
                v = lift[b][v];
            }
        return {cost, v};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<pair<int, int>> intervals, overnight;
    while (n--) {
        int s, e;
        cin >> s >> e;

        if (s < e) intervals.emplace_back(s, e);
        else overnight.emplace_back(s, e);
    }
    if (overnight.empty()) {
        cout << -1;
        exit(0);
    }

    sort(intervals.begin(), intervals.end(), [&](auto p1, auto p2) { return p1.first != p2.first ? p1.first < p2.first : p1.second > p2.second; });
    vector<int> l, r;
    for (auto [s, e] : intervals)
        if (r.empty() || e > r.back()) {
            l.emplace_back(s);
            r.emplace_back(e);
        }

    int k = l.size();
    vector<int> next(k + 1, k), w(k + 1, 1);
    w[k] = 0;
    for (int i = 0, j = 0; i < k; i++) {
        j = max(j, i);
        for (; j + 1 < k && l[j + 1] <= r[i]; j++);
        if (j > i) next[i] = j;
    }

    JumpTable<int> jt(k + 1, k + 1, next, w);
    int least = INT_MAX;
    for (auto [s, e] : overnight) {
        int i = upper_bound(l.begin(), l.end(), e) - l.begin() - 1;
        if (!~i || r[i] <= e) continue;
        if (r[i] >= s) {
            least = min(least, 2);
            continue;
        }

        auto [cost, v] = jt.jump_down(i, lower_bound(r.begin(), r.end(), s) - r.begin() - 1);
        int u = next[v];
        if (u < k && r[u] >= s) least = min(least, cost + 3);
    }
    cout << (least == INT_MAX ? -1 : least);
}
