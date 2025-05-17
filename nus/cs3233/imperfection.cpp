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

    int find_kth(int k) {
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

    int n;
    cin >> n;

    auto total = 0LL;
    vector<int> a(n), b(n);
    for (int &ai : a) cin >> ai;
    for (int &bi : b) {
        cin >> bi;

        total += bi;
    }

    auto temp = a;
    sort(a.begin(), a.end());
    a.erase(unique(a.begin(), a.end()), a.end());
    int m = a.size();
    vector<vector<int>> costs(m);
    vector<int> count(m, 0);
    vector<long long> sum(m, 0);
    for (int i = 0; i < n; i++) {
        int j = lower_bound(a.begin(), a.end(), temp[i]) - a.begin();
        costs[j].emplace_back(b[i]);
        count[j]++;
        sum[j] += b[i];
    }

    int s = *max_element(b.begin(), b.end());
    FenwickTree<int> fw_c(s + 1);
    FenwickTree<long long> fw_s(s + 1);
    auto c = LLONG_MAX;
    int pref_count = 0;
    for (int j = 0; j < m; j++) {
        auto extra = 0LL;
        if (pref_count + 1 > count[j]) {
            int i = fw_c.find_kth(pref_count - count[j] + 1);
            extra = fw_s.pref_sum(i - 1) + (long long) i * (pref_count - count[j] + 1 - fw_c.pref_sum(i - 1));
        }

        total -= sum[j];
        c = min(c, total + extra);
        for (int bi : costs[j]) {
            fw_c.update(bi, 1);
            fw_s.update(bi, bi);
        }
        pref_count += count[j];
    }
    cout << c;
}
