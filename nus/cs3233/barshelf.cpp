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

    vector<int> h(n);
    for (int &hi : h) cin >> hi;

    auto deduped = h;
    sort(deduped.begin(), deduped.end());
    deduped.erase(unique(deduped.begin(), deduped.end()), deduped.end());

    auto messy = 0LL;
    FenwickTree<long long> fw1(deduped.size() + 1), fw2(deduped.size() + 1);
    for (int i = n - 1; ~i; i--) {
        int j = lower_bound(deduped.begin(), deduped.end(), h[i]) - deduped.begin(),
            k = upper_bound(deduped.begin(), deduped.end(), h[i] / 2) - deduped.begin();

        auto ps1 = fw1.pref_sum(k), ps2 = fw2.pref_sum(k);
        fw1.update(j + 1, 1);
        fw2.update(j + 1, ps1);
        messy += ps2;
    }
    cout << messy;
}
