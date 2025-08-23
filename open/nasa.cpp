#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, p, q, m;
    cin >> n >> p >> q >> m;

    int t_max = 0;
    vector<pair<int, int>> transmissions(m);
    for (auto &[s, t] : transmissions) {
        cin >> s >> t;

        t_max = max(t_max, t);
    }

    vector<int> y(n);
    int l = max((n + q - 1) / q, t_max) - 1, r = n / p + 1, mid;
    while (l + 1 < r) {
        mid = l + (r - l) / 2;

        auto valid = [&](int a) -> bool {
            if (a < t_max || !(n / q <= a && a <= n / p)) return false;

            vector<int> s_min(a + 2, n + 1), s_max(a + 2, 0);
            for (auto [s, t] : transmissions) {
                if (a < t) return false;
                s_min[t] = min(s_min[t], s);
                s_max[t] = max(s_max[t], s);
            }

            vector<int> dp_l(a + 1, n), dp_r(a + 1, n);
            for (int t = a - 1; ~t; t--) {
                dp_l[t] = max(0, dp_l[t + 1] - q);
                dp_r[t] = min({n, dp_r[t + 1] - p, s_min[t + 1] - 1});
                if (dp_l[t] > dp_r[t]) return false;
            }
            if (!(dp_l[0] <= 0 && 0 <= dp_r[0])) return false;

            vector<int> indices(a + 1, 0);
            for (int t = 1; t <= a; t++) {
                if (s_min[t] != n + 1 && indices[t - 1] > s_min[t] - 1) return false;
                int i = max({indices[t - 1] + p, s_max[t], dp_l[t]}), j = min({indices[t - 1] + q, n, dp_r[t]});
                if (i > j) return false;
                indices[t] = j;
            }
            if (indices[a] != n) return false;

            for (int i = 0, t = 1; i < n; i++) {
                while (i >= indices[t]) t++;
                y[i] = t;
            }
            return true;
        };

        if (valid(mid)) l = mid;
        else r = mid;
    }
    cout << l << "\n";
    for (int yi : y) cout << yi << " ";
}
