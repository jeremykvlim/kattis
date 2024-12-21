#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    vector<T> BIT;

    void update(int i, T v) {
        for (; i < BIT.size(); i += i & -i)
            BIT[i] += v;
    }

    T pref_sum(int i) {
        T sum = 0;
        for (; i; i &= (i - 1)) sum += BIT[i];
        return sum;
    }

    FenwickTree(int n) : BIT(n, 0) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    auto sum = 0LL;
    vector<int> a(n);
    FenwickTree<long long> fw(n + 1);
    for (int i = 0; i < n; i++) {
        cin >> a[i];

        sum += a[i];
        fw.update(i + 1, a[i]);
    }

    while (q--) {
        int i, x;
        cin >> i >> x;

        int diff = x - a[i];
        a[i] = x;
        sum += diff;
        fw.update(i + 1, diff);

        int l = -1, r = n, m;
        long long pref_l, pref_r;
        while (l + 1 < r) {
            m = l + (r - l) / 2;

            pref_l = fw.pref_sum(m);
            pref_r = sum - a[m] - pref_l;

            if (pref_l < pref_r) l = m;
            else r = m;
        }

        if (!~l) cout << "0\n";
        else {
            pref_l = fw.pref_sum(l + 1);
            pref_r = sum - a[l + 1] - pref_l;
            auto diff1 = (pref_l - pref_r) + ((a[l + 1] & 1) * (pref_l == pref_r ? 1 : -1));
            pref_l -= a[l];
            pref_r += a[l + 1];
            auto diff2 = (pref_r - pref_l) + ((a[l] & 1) * (pref_l == pref_r ? 1 : -1));
            cout << (diff1 < diff2 ? l + 1 : l) << "\n";
        }
    }
}
