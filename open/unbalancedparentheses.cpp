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

    int n, k;
    string s;
    cin >> n >> k >> s;

    int sum_neg = 0;
    vector<int> c(n);
    for (int &ci : c) {
        cin >> ci;

        if (ci < 0) sum_neg += ci;
    }

    if (n & 1) {
        cout << sum_neg;
        exit(0);
    }

    if (k >= n / 2 + 1) {
        cout << "?";
        exit(0);
    }

    int count1 = 0;
    vector<bool> neg(n, false);
    for (int i = 0; i < n; i++) {
        int delta = (s[i] == ')') ? c[i] : -c[i];
        if (delta < 0) {
            neg[i] = true;
            count1++;
        }
        c[i] = abs(c[i]);
    }

    FenwickTree<int> fw1(1003), fw2(1003);
    auto add = [&](int ci, int delta) {
        fw1.update(ci + 1, delta);
        fw2.update(ci + 1, delta * ci);
    };
    for (int i = 0; i < n; i++)
        if (!neg[i]) add(c[i], 1);

    int sum = 1e9;
    for (int i = 0, count2 = 0; i <= n; i++) {
        int moves = k + n / 2 + 1 - count1 + 2 * count2 - i - (i & 1), j = fw1.lower_bound(moves);
        sum = min(sum, sum_neg + (moves - fw1.pref_sum(j - 1)) * (j - 1) + fw2.pref_sum(j - 1));
        if (i == n) break;

        if (neg[i]) {
            add(c[i], 1);
            count2++;
        } else add(c[i], -1);
    }
    cout << sum;
}
