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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> pos(n + 1);
    for (int i = 1; i <= n; i++) {
        int h;
        cin >> h;

        pos[h] = i;
    }

    auto formations = 0LL;
    FenwickTree<int> count(n);
    FenwickTree<long long> sum(n);
    for (int h = n; h; h--) {
        int k = pos[h], c = count.pref_sum(k - 1);
        auto s = sum.pref_sum(k - 1);

        formations += (long long) c * (k - 1) - s - (long long) c * (c - 1) / 2;
        count.update(k, 1);
        sum.update(k, k);
    }
    cout << formations;
}
