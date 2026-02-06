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

    int upper_bound(T k) {
        int i = 0;
        for (int m = bit_ceil(BIT.size()); m; m >>= 1)
            if (i + m < BIT.size() && BIT[i + m] <= k) {
                i += m;
                k -= BIT[i];
            }
        return i + 1;
    }

    FenwickTree(int n) : BIT(n, 0) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<int> a(n + 1);
    FenwickTree<long long> fw(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> a[i];

        fw.update(i, a[i]);
    }

    while (q--) {
        int t, x, y;
        cin >> t >> x >> y;

        if (t == 1) {
            fw.update(x, y - a[x]);
            a[x] = y;
        } else cout << max(0, fw.upper_bound(fw.pref_sum(x - 1) + y) - x) << "\n";
    }
}
