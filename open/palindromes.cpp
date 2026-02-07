#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    vector<T> BIT;

    void update(int i, T v) {
        for (; i && i < BIT.size(); i += i & -i) BIT[i] += v;
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

vector<int> manacher(const string &s) {
    int n = s.size();
    if (n <= 1) return {};

    vector<int> dp(2 * n - 1, 0);
    for (int k = 0, l = -1, r = -1; k < 2 * n - 1; k++) {
        int i = (k + 1) >> 1, j = k >> 1, p = i >= r ? 0 : min(r - i, dp[2 * (l + r) - k]);
        while (j + p + 1 < n && i - p - 1 >= 0 && s[j + p + 1] == s[i - p - 1]) p++;

        if (r < j + p) {
            r = j + p;
            l = i - p;
        }

        dp[k] = p;
    }
    return dp;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;
    int n = s.size();

    auto dp = manacher(s);
    vector<int> radius_odd(n, 0), radius_even(n, 0);
    if (n == 1) radius_odd[0] = 1;
    else
        for (int i = 0; i < n; i++) {
            radius_odd[i] = dp[2 * i] + 1;
            if (i) radius_even[i] = dp[2 * i - 1];
        }

    vector<long long> pref_odd(n + 1, 0), pref_even(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        pref_odd[i] = pref_odd[i - 1] + radius_odd[i - 1];
        pref_even[i] = pref_even[i - 1] + radius_even[i - 1];
    }

    int m;
    cin >> m;

    vector<int> sum(m);
    vector<vector<int>> indices_l(n + 2), indices_r(n + 2);
    for (int q = 0; q < m; q++) {
        int l, r;
        cin >> l >> r;

        sum[q] = l + r;
        indices_l[l].emplace_back(q);
        indices_r[r].emplace_back(q);
    }

    vector<long long> count(m, 0);
    auto sweep_l = [&](auto &radius, auto &pref, bool parity) {
        vector<pair<int, int>> order;
        for (int i = 1; i <= n; i++) {
            int r = radius[i - 1];
            if (parity && !r) continue;
            order.emplace_back(i - r + 1 - parity, i);
        }
        sort(order.begin(), order.end());

        FenwickTree<long long> fw1(n + 1), fw2(n + 1);
        for (int l = 1, j = 0; l <= n; l++) {
            for (; j < order.size() && order[j].first < l; j++) {
                int i = order[j].second;
                fw1.update(i, 1);
                fw2.update(i, radius[i - 1] - i - 1 + parity);
            }

            for (int q : indices_l[l]) {
                int mid = (sum[q] + parity) / 2;
                if (l + parity <= mid) count[q] += pref[mid] - pref[l - 1 + parity] - fw2.range_sum_query(l - 1 + parity, mid) - fw1.range_sum_query(l - 1 + parity, mid) * l;
            }
        }
    };

    auto sweep_r = [&](auto &radius, auto &pref, bool parity) {
        vector<pair<int, int>> order;
        for (int i = 1; i <= n; i++) {
            int r = radius[i - 1];
            if (parity && !r) continue;
            order.emplace_back(i + r - 1, i);
        }
        sort(order.begin(), order.end(), [](const auto &p1, const auto &p2) {
            if (p1.first != p2.first) return p1.first > p2.first;
            return p1.second < p2.second;
        });

        FenwickTree<long long> fw1(n + 1), fw2(n + 1);
        for (int r = n, j = 0; r; r--) {
            for (; j < order.size() && order[j].first > r; j++) {
                int i = order[j].second;
                fw1.update(i, 1);
                fw2.update(i, radius[i - 1] + i - 1);
            }
            
            for (int qi : indices_r[r]) {
                int mid = (sum[qi] + parity) / 2;
                if (mid <= r - 1) count[qi] += pref[r] - pref[mid] - fw2.range_sum_query(mid, r) + fw1.range_sum_query(mid, r) * r;
            }
        }
    };
    sweep_l(radius_odd, pref_odd, false);
    sweep_l(radius_even, pref_even, true);
    sweep_r(radius_odd, pref_odd, false);
    sweep_r(radius_even, pref_even, true);

    for (int c : count) cout << c << "\n";
}
