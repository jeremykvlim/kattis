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

    int n, K, D, q;
    cin >> n >> K >> D >> q;

    vector<int> d(n);
    for (int &di : d) cin >> di;

    vector<int> k(n);
    vector<vector<int>> indices(K);
    for (int i = 0; i < n; i++) {
        cin >> k[i];

        indices[k[i]].emplace_back(i);
    }

    vector<vector<int>> pos(K);
    for (int ki = 0; ki < K; ki++) {
        sort(indices[ki].begin(), indices[ki].end(), [&](int i, int j) { return d[i] < d[j]; });
        pos[ki].resize(indices[ki].size());
        for (int j = 0; j < indices[ki].size(); j++) pos[ki][j] = d[indices[ki][j]];
    }

    vector<int> next(n);
    vector<long long> w(n);
    for (int i = 0; i < n; i++) {
        int ki = (k[i] + 1) % K;
        auto it = lower_bound(pos[ki].begin(), pos[ki].end(), d[i]);
        if (it != pos[ki].end()) {
            int l = it - pos[ki].begin();
            next[i] = indices[ki][l];
            w[i] = *it - d[i];
        } else {
            next[i] = indices[ki][0];
            w[i] = (D - d[i]) + pos[ki][0];
        }
    }

    JumpTable<long long> jt(K, n, next, w);
    while (q--) {
        int dq, kq, nq;
        cin >> dq >> kq >> nq;

        if (!nq) {
            cout << "0\n";
            continue;
        }

        auto dist = 0LL;
        auto it = lower_bound(pos[kq].begin(), pos[kq].end(), dq);
        int j;
        if (it != pos[kq].end()) {
            int l = it - pos[kq].begin();
            j = indices[kq][l];
            dist = *it - dq;
        } else {
            j = indices[kq][0];
            dist = (D - dq) + pos[kq][0];
        }
        cout << dist + jt.jump_up(j, nq - 1).first << "\n";
    }
}
