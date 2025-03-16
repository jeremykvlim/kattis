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
        T v = numeric_limits<T>::max();
        for (; i; i &= i - 1) v = f(v, BIT[i]);
        return v;
    }

    FenwickTree(int n, function<T(T, T)> func) : BIT(n, numeric_limits<T>::max()), f(move(func)) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long b, p;
    int d, n;
    cin >> b >> p >> d >> n;

    vector<long long> a(n + 1, 0), r(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        cin >> a[i];

        r[i] = a[i] % p;
    }
    sort(r.begin(), r.end());
    r.erase(unique(r.begin(), r.end()), r.end());

    int m = r.size();
    FenwickTree<long long> fw(m + 1, [](long long x, long long y) {return min(x, y);});
    vector<long long> dp(n + 1, 0);
    auto damage = LLONG_MAX, lowest = LLONG_MAX;
    for (int i = 0; i <= n; i++) {
        auto index = [&](auto v) {
            return m - (lower_bound(r.begin(), r.end(), v) - r.begin());
        };

        if (i) dp[i] = min(lowest, fw.range_query(index((a[i] - 1) % p + 1)) - (d + p)) + (a[i] - 1) / p * (d + p) + p;
        lowest = min(lowest, dp[i] - (a[i] / p) * (d + p));
        fw.update(index(a[i] % p), dp[i] - (a[i] / p) * (d + p));
    }
    for (int i = 0; i <= n; i++) damage = min(damage, dp[i] + b - a[i] + (b - a[i] - 1) / p * d);
    cout << damage;
}
