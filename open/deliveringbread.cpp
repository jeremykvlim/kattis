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
    for (int ki = 0; ki < K; ki++) {
        sort(indices[ki].begin(), indices[ki].end(), [&](int i, int j) { return d[i] < d[j]; });
        pos[ki].resize(indices[ki].size());
        for (int j = 0; j < indices[ki].size(); j++) pos[ki][j] = d[indices[ki][j]];
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
    vector<int> block_count(n), jump(n);
    vector<long long> block_step(n);
    for (int l = 0; l < K; l += blocks) {
        int r = min(K, l + blocks);
        for (int c = r - 1; c >= l; c--)
            for (int i : indices[c]) {
                int j = next[i];
                if (c + 1 < r) {
                    jump[i] = jump[j];
                    block_step[i] = step[i] + block_step[j];
                    block_count[i] = block_count[j] + 1;
                } else {
                    jump[i] = j;
                    block_step[i] = step[i];
                    block_count[i] = 1;
                }
            }
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
        while (m >= block_count[j]) {
            m -= block_count[j];
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
