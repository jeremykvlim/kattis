#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    vector<T> BIT;

    void update(int i, T v) {
        for (; i && i < BIT.size(); i += i & -i) BIT[i] += v;
    }

    void update(int l, int r, T v) {
        update(l, v);
        update(r, -v);
    }

    T pref_sum(int i) {
        T sum = 0;
        for (; i; i &= i - 1) sum += BIT[i];
        return sum;
    }

    T range_sum_query(int l, int r) {
        if (l >= r) return 0;
        return pref_sum(r) - pref_sum(l);
    }

    FenwickTree(int n) : BIT(n, 0) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, s;
    cin >> n >> m >> s;

    vector<pair<int, int>> edges(m);
    for (auto &[d, t] : edges) cin >> d >> t;
    sort(edges.rbegin(), edges.rend());

    FenwickTree<int> fw(n + 2);
    for (int i = 1; i <= n; i++) fw.update(i, i + 1, min(abs(i - s), n - abs(i - s)));

    for (auto [d, t1] : edges) {
        auto index = [&](int i) {
            return (i + n - 1) % n + 1;
        };

        int t2 = index(t1 + 1), dp1 = fw.pref_sum(t1), dp2 = fw.pref_sum(t2);
        if (dp1 == dp2) continue;

        int dp = min(dp1, dp2), t = dp1 > dp2 ? t1 : t2, dir = dp1 > dp2 ? -1 : 1;
        fw.update(t1, t1 + 1, dir);
        fw.update(t2, t2 + 1, -dir);
        if (dp > fw.pref_sum(index(t - 2 * dir))) fw.update(index(t - dir), index(t - dir) + 1, -1);

        int l = 0, r = n, mid;
        while (l + 1 < r) {
            mid = l + (r - l) / 2;

            if (fw.pref_sum(index(t + dir * mid)) > dp + mid) l = mid;
            else r = mid;
        }

        if (l) {
            auto update = [&](int l, int r) {
                if (l <= r) fw.update(l, r + 1, -1);
                else {
                    fw.update(1, r + 1, -1);
                    fw.update(l, n + 1, -1);
                }
            };

            if (~dir) update(index(t + 1), index(t + l));
            else update(index(t - l), index(t - 1));
        }
    }
    for (int i = 1; i <= n; i++) cout << fw.pref_sum(i) << "\n";
}
