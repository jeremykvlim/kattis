#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    vector<T> BIT;
    function<T(T, T)> f;
    T unit;

    void update(int i, T v) {
        for (; i && i < BIT.size(); i += i & -i) BIT[i] = f(BIT[i], v);
    }

    T range_query(int i) {
        T v = unit;
        for (; i; i &= i - 1) v = f(v, BIT[i]);
        return v;
    }

    void reset() {
        fill(BIT.begin(), BIT.end(), unit);
    }

    FenwickTree(int n, function<T(T, T)> func, T unit) : BIT(n, unit), f(move(func)), unit(unit) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<int> order(n + 1);
    for (int i = 1; i <= n; i++) cin >> order[i];

    int count = 0;
    vector<array<int, 3>> dp1(n + 1), dp2(n + 1);
    vector<bool> used(n + 1, false);
    vector<int> prev(n + 1, 0);
    FenwickTree<int> fw(n + 1, [&](int i, int j) { return !i || !j ? i ^ j : (dp2[i][0] > dp2[j][0] ? i : j); }, 0);
    for (int _ = 0; _ < 4; _++) {
        for (int i = 1; i <= n; i++) {
            dp1[i] = {0, i, 0};
            dp2[i] = {0, i, 1};
        }

        vector<bool> stale(n + 1, true);
        for (int l = 1; l <= n;) {
            fw.reset();

            int i = l;
            for (l = n + 1; i <= n; i++) {
                int j = fw.range_query(max(0, order[i] - k)), temp1 = dp1[i][0], temp2 = dp2[i][0];
                if (j && dp1[i][0] < dp2[j][0]) dp1[i] = {dp2[j][0], j, 1};
                if (used[i] && dp1[i][0] < dp2[i][0] - 1) dp1[i] = {dp2[i][0] - 1, i, 1};
                if (!used[i] && dp2[i][0] < dp1[i][0] + 1) dp2[i] = {dp1[i][0] + 1, i, 0};

                if (dp1[i][0] > temp1)
                    for (int v = i, p = prev[v]; p && dp1[v][0] > dp2[p][0]; v = p, p = prev[p]) {
                        dp2[p] = {dp1[v][0], v, 0};
                        fw.update(order[p], p);

                        if (!stale[p]) {
                            stale[p] = true;
                            l = min(l, p);
                        }

                        if (dp1[p][0] >= dp2[p][0] - 1) break;
                        dp1[p] = {dp2[p][0] - 1, p, 1};
                    }

                if (stale[i] || dp2[i][0] > temp2) fw.update(order[i], i);
                stale[i] = false;
            }
        }

        int j = 1;
        for (int i = 2; i <= n; i++)
            if (dp2[j][0] < dp2[i][0]) j = i;

        count += dp2[j][0];
        vector<pair<int, int>> path;
        for (int v = j, type = 1;;) {
            path.emplace_back(v, type);
            int u = type ? dp2[v][1] : dp1[v][1], t = type ? dp2[v][2] : dp1[v][2];
            if (v == u && type == t) break;
            v = u;
            type = t;
        }
        reverse(path.begin(), path.end());

        for (int i = 1; i < path.size(); i++) {
            auto [u, u_type] = path[i - 1];
            auto [v, v_type] = path[i];

            if (u == v) used[u] = !u_type;
            else if (u < v) prev[v] = u;
            else if (prev[u] == v) prev[u] = 0;
        }
    }
    cout << count;
}
