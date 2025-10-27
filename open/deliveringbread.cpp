#include <bits/stdc++.h>
using namespace std;

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
    for (int i = 0; i < K; i++) {
        sort(indices[i].begin(), indices[i].end(), [&](int a, int b) { return d[a] < d[b]; });
        pos[i].resize(indices[i].size());
        for (int j = 0; j < indices[i].size(); j++) pos[i][j] = d[indices[i][j]];
    }

    vector<int> next(n);
    vector<long long> step(n);
    for (int i = 0; i < n; i++) {
        int j = (k[i] + 1) % K;
        auto it = lower_bound(pos[j].begin(), pos[j].end(), d[i]);
        if (it != pos[j].end()) {
            int l = it - pos[j].begin();
            next[i] = indices[j][l];
            step[i] = *it - d[i];
        } else {
            next[i] = indices[j][0];
            step[i] = (D - d[i]) + pos[j][0];
        }
    }

    int blocks = ceil(sqrt(K));
    vector<int> jump(n);
    vector<long long> block_step(n);
    for (int i = 0; i < n; i++) {
        int j = i;
        for (int _ = 0; _ < blocks; _++) {
            block_step[i] += step[j];
            j = next[j];
        }
        jump[i] = j;
    }

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

        int m = nq - 1;
        while (m >= blocks) {
            m -= blocks;
            dist += block_step[j];
            j = jump[j];
        }
        while (m) {
            m--;
            dist += step[j];
            j = next[j];
        }
        cout << dist << "\n";
    }
}
