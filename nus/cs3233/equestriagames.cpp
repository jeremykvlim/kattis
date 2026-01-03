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
        int steps = 0;
        for (int b = 0; b < lg; b++)
            if ((k >> b) & 1) {
                cost += sum[b][v];
                steps += 1 << b;
                v = lift[b][v];
            }
        return {cost, steps};
    }

    pair<T, int> jump_down(int v, T bound) const {
        T cost = 0;
        int steps = 0;
        for (int b = lg - 1; ~b; b--)
            if (lift[b][v] <= bound) {
                cost += sum[b][v];
                steps += 1 << b;
                v = lift[b][v];
            }
        return {cost, steps};
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, c;
    cin >> n >> k >> c;

    vector<int> a(n);
    for (int &ai : a) cin >> ai;

    auto temp = a;
    sort(temp.begin(), temp.end());
    temp.erase(unique(temp.begin(), temp.end()), temp.end());
    for (int &ai : a) ai = lower_bound(temp.begin(), temp.end(), ai) - temp.begin();

    int m = 2 * n;
    a.resize(m);
    for (int i = n; i < m; i++) a[i] = a[i - n];

    vector<int> freq(temp.size(), 0), next(m + 2, m + 1);
    for (int l = 1, r = 1, count = 0; l <= m; l++) {
        for (; r <= m && count < c; r++)
            if (!freq[a[r - 1]]++) count++;

        if (count >= c) next[l] = min(m + 1, max(r, l + k));
        if (!--freq[a[l - 1]]) count--;
    }

    JumpTable<int> jt(n, m + 2, next, vector<int>(m + 2, 1));
    int sold = 0;
    for (int l = 1; l <= n; l++) sold = max(sold, jt.jump_down(l, l + n).second);
    cout << sold;
}