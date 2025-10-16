#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    vector<T> BIT;
    function<T(T, T)> f;

    void update(int i, T v) {
        for (; i && i < BIT.size(); i += i & -i) BIT[i] = f(BIT[i], v);
    }

    T range_query(int i) {
        T v = 0;
        for (; i; i &= i - 1) v = f(v, BIT[i]);
        return v;
    }

    FenwickTree(int n, function<T(T, T)> func) : BIT(n, 0), f(move(func)) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> a(n + 1), a_min(n + 1), a_max(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> a[i];

        a_min[i] = a_max[i] = a[i];
    }

    int q;
    cin >> q;

    vector<vector<pair<int, int>>> queries(n + 1);
    for (int i = 1; i <= q; i++) {
        int l, r;
        cin >> l >> r;

        queries[r].emplace_back(l, i);
    }

    vector<int> len(q + 1);
    FenwickTree<int> fw(n + 1, [](int x, int y) { return max(x, y); });
    for (int r = 1; r <= n; r++) {
        for (int i = r; i; i--) {
            a_min[i] = min(a_min[i], a[r]);
            a_max[i] = max(a_max[i], a[r]);
            if (a_min[i] < a[r] && a_max[i] > a[r]) break;
            if ((a_min[i] == a[i] && a_max[i] == a[r]) || (a_max[i] == a[i] && a_min[i] == a[r])) fw.update(n - i + 1, r - i + 1);
        }
        for (auto &[l, i] : queries[r]) len[i] = fw.range_query(n - l + 1);
    }

    for (int i = 1; i <= q; i++) cout << len[i] << "\n";
}
