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

    int lg = __lg(K) + 1;
    vector<vector<int>> lift(lg, vector<int>(n));
    vector<vector<long long>> dist_lift(lg, vector<long long>(n));
    for (int i = 0; i < n; i++) {
        auto it = lower_bound(pos[(k[i] + 1) % K].begin(), pos[(k[i] + 1) % K].end(), d[i]);
        if (it != pos[(k[i] + 1) % K].end()) {
            int l = it - pos[(k[i] + 1) % K].begin();
            lift[0][i] = indices[(k[i] + 1) % K][l];
            dist_lift[0][i] = *it - d[i];
        } else {
            lift[0][i] = indices[(k[i] + 1) % K][0];
            dist_lift[0][i] = (D - d[i]) + pos[(k[i] + 1) % K][0];
        }
    }

    for (int i = 1; i < lg; i++)
        for (int j = 0; j < n; j++) {
            lift[i][j] = lift[i - 1][lift[i - 1][j]];
            dist_lift[i][j] = dist_lift[i - 1][j] + dist_lift[i - 1][lift[i - 1][j]];
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

        for (int i = 0; i < lg; i++)
            if (((nq - 1) >> i) & 1) {
                dist += dist_lift[i][j];
                j = lift[i][j];
            }
        cout << dist << "\n";
    }
}
