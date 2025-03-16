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

    FenwickTree(int n) : BIT(n, 0) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    map<int, int> indices;
    vector<int> h(n);
    for (int &hi : h) {
        cin >> hi;

        indices[hi];
    }

    int unique = 0;
    for (auto &[hi, i] : indices) i = unique++;
    indices[INT_MAX] = unique;

    FenwickTree<long long> fw1(unique + 1), fw2(unique + 1);
    auto messy = 0LL;
    for (int i = n - 1; ~i; i--) {
        int j = indices[h[i]], k = indices.upper_bound(h[i] / 2)->second;

        auto ps1 = fw1.pref_sum(k), ps2 = fw2.pref_sum(k);
        fw1.update(j + 1, 1);
        fw2.update(j + 1, ps1);
        messy += ps2;
    }

    cout << messy;
}
