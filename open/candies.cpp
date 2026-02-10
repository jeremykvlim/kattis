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

    vector<int> a(n);
    for (int &ai : a) cin >> ai;
    sort(a.begin(), a.end());

    int m;
    cin >> m;

    FenwickTree<long long> fw(n + 2);
    while (m--) {
        int b;
        cin >> b;

        int l = -1, r = n, mid;
        while (l + 1 < r) {
            mid = l + (r - l) / 2;

            if (a[mid] - fw.pref_sum(mid + 1) >= b) r = mid;
            else l = mid;
        }

        cout << n - r << "\n";
        if (n > r) fw.update(r + 1, 1);
    }
}
