#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    if (n == 1) {
        cout << "! 1";
        exit(0);
    }

    auto query = [&](const vector<int> &q) {
        cout << "? ";
        for (int i = 1; i <= n; i++) cout << q[i] << " ";
        cout << "\n" << flush;

        int s;
        cin >> s;
        return s;
    };

    auto build = [&](const vector<pair<int, int>> &v) {
        vector<int> q(n + 1), q_inv(n + 1);
        iota(q.begin(), q.end(), 0);
        iota(q_inv.begin(), q_inv.end(), 0);

        for (auto [i, qi] : v)
            if (i != q_inv[qi]) {
                int j = q_inv[qi];
                swap(q[i], q[j]);
                q_inv[q[i]] = i;
                q_inv[q[j]] = j;
            }
        return q;
    };

    auto diff = [&](int i, int j, int m) {
        auto q = build({{i, 1}, {j, m}});
        int temp = query(q);
        swap(q[i], q[j]);
        return temp - query(q);
    };

    int l = 1, r = n + 1, m;
    while (l + 1 < r) {
        m = l + (r - l) / 2;

        if (!diff(1, 2, m)) l = m;
        else r = m;
    }

    int j = 1 + (diff(1, 2, n) >= 0), size = ceil(sqrt(n)), blocks = (n + size - 1) / size;
    if (l < size)
        for (int i = 1; i <= n; i++)
            if (i != j) {
                int d = diff(i, j, n) / 2;
                if (l + d >= size) {
                    l += d;
                    j = i;
                    break;
                }
            }

    vector<vector<int>> qs(blocks);
    vector<int> base(blocks);
    for (int b = 0; b < blocks; b++) {
        vector<pair<int, int>> v{{j, n}};
        for (int i = b * size + 1; i <= min(n, (b + 1) * size); i++)
            if (i != j) v.emplace_back(i, i - b * size);
        base[b] = query(qs[b] = build(v));
    }

    vector<int> p(n + 1, l);
    for (int i = 1; i <= n; i++)
        if (i != j) {
            int b = (i - 1) / size, delta = qs[b][i] - l;
            swap(qs[b][i], qs[b][j]);
            int d = base[b] - query(qs[b]);
            swap(qs[b][i], qs[b][j]);
            p[i] += d / 2 != delta ? d / 2 : diff(i, j, n) / 2;
        }

    cout << "! ";
    for (int i = 1; i <= n; i++) cout << p[i] << " ";
}
