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

    int kth(int k) {
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

    int sum = 0, d_max = 0;
    vector<int> d(n);
    for (int &di : d) {
        cin >> di;

        sum += di;
        d_max = max(d_max, di);
    }

    if (sum < 2 * n - 2) {
        cout << "Omogulegt!";
        exit(0);
    }

    vector<stack<int>> indices(d_max + 1);
    for (int i = 0; i < n; i++)
        if (d[i]) indices[d[i]].emplace(i);

    FenwickTree<int> fw(d_max + 1);
    for (int degree = 1; degree <= d_max; degree++)
        if (!indices[degree].empty()) fw.update(degree, indices[degree].size());

    int total = fw.pref_sum(d_max);

    auto add = [&](int degree, int v) {
        if (!degree) return;
        indices[degree].emplace(v);
        fw.update(degree, 1);
        total++;
    };

    auto remove = [&](int degree) {
        int v = indices[degree].top();
        indices[degree].pop();
        fw.update(degree, -1);
        total--;
        return v;
    };

    vector<pair<int, int>> edges;
    while (total) {
        int d1 = fw.kth(1), b = remove(d1);
        vector<pair<int, int>> undo;
        while (d1--) {
            if (!total) {
                cout << "Omogulegt!";
                exit(0);
            }

            int d2 = fw.kth(total), a = remove(d2);
            edges.emplace_back(a + 1, b + 1);
            undo.emplace_back(d2 - 1, a);
        }
        for (auto &[degree, v] : undo) add(degree, v);
    }

    cout << n << " " << edges.size() << "\n";
    for (auto &[a, b] : edges) cout << a << " " << b << "\n";
}
