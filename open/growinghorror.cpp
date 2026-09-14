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

    int lower_bound(T k) {
        int i = 0;
        for (int m = bit_ceil(BIT.size()); m; m >>= 1)
            if (i + m < BIT.size() && BIT[i + m] < k) {
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

    auto total = 0LL;
    vector<int> w(n);
    FenwickTree<int> count(n + 1);
    FenwickTree<long long> sum(n + 1);
    for (int &wi : w) {
        cin >> wi;

        total += wi;
        count.update(wi, 1);
        sum.update(wi, wi);
    }

    while (q--) {
        int i, x;
        cin >> i >> x;
        i--;

        count.update(w[i], -1);
        sum.update(w[i], -w[i]);
        total -= w[i];
        w[i] = x;
        total += w[i];
        sum.update(w[i], w[i]);
        count.update(w[i], 1);
        int l = count.lower_bound(1), r = count.lower_bound(n), m = l + (r - l) / 2, c = count.pref_sum(m);
        auto s = sum.pref_sum(m);
        cout << (long long) c * r - s + total - s - (long long) (n - c) * l - (r - l) << "\n";
    }
}