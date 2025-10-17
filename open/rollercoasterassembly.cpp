#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    vector<T> BIT;
    function<T(T, T)> f;

    void update(int i, T v) {
        for (; i && i < BIT.size(); i += i & -i) BIT[i] = f(BIT[i], v);
    }

    T range_query(int i) {
        T v = numeric_limits<T>::lowest();
        for (; i; i &= i - 1) v = f(v, BIT[i]);
        return v;
    }

    FenwickTree(int n, function<T(T, T)> func) : BIT(n, numeric_limits<T>::lowest()), f(move(func)) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n), b(n);
    for (int i = 0; i < n; i++) cin >> a[i] >> b[i];

    auto deduped = b;
    sort(deduped.begin(), deduped.end());
    deduped.erase(unique(deduped.begin(), deduped.end()), deduped.end());

    vector<long long> pref(n, 0);
    for (int i = 0; i < n - 1; i++) pref[i + 1] = pref[i] + min(a[i + 1], b[i]);

    int m = deduped.size();
    FenwickTree<long long> fw_l(m + 1, [](auto x, auto y) { return max(x, y); }), fw_r(m + 1, [](auto x, auto y) { return max(x, y); });
    vector<long long> dp(n + 2, 0), max_l(m, LLONG_MIN), max_r(m, LLONG_MIN);
    for (int i = n - 1; ~i; i--) {
        if (i < n - 1) {
            int k = lower_bound(deduped.begin(), deduped.end(), b[i + 1]) - deduped.begin();
            auto l = pref[i + 1] + dp[i + 2];
            if (max_l[k] < l) {
                max_l[k] = l;
                fw_l.update(m - k, l);
            }

            auto r = l + b[i + 1];
            if (max_r[k] < r) {
                max_r[k] = r;
                fw_r.update(k + 1, r);
            }
        }

        int k = lower_bound(deduped.begin(), deduped.end(), a[i]) - deduped.begin();
        auto l = fw_l.range_query(m - k);
        if (l != LLONG_MIN) l -= pref[i] - a[i];

        auto r = fw_r.range_query(k);
        if (r != LLONG_MIN) r -= pref[i];
        dp[i] = max(l, r);
    }
    cout << dp[0];
}
