#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    vector<T> BIT;

    void update(int i, T v) {
        for (; i && i < BIT.size(); i += i & -i) BIT[i] += v;
    }

    void update(int l, int r, T v) {
        update(l, v);
        update(r, -v);
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

    int n, m;
    cin >> n >> m;

    int size = ceil(sqrt(n)), blocks = (n + size - 1) / size;
    vector<int> a(n);
    vector<FenwickTree<int>> bit(501, blocks + 1);
    for (int i = 0; i < n; i++) {
        cin >> a[i];

        bit[a[i]].update(i / size + 1, 1);
    }

    while (m--) {
        int type;
        cin >> type;

        if (!type) {
            int p, x;
            cin >> p >> x;
            p--;

            if (a[p] == x) continue;
            bit[a[p]].update(p / size + 1, -1);
            bit[x].update(p / size + 1, 1);
            a[p] = x;
        } else {
            int l, r, x;
            cin >> l >> r >> x;
            l--;
            r--;

            array<int, 501> count{};
            int bl = l / size, br = r / size;
            for (int i = l; i <= min(r, (bl + 1) * size - 1); i++) count[a[i]]++;
            if (bl < br)
                for (int i = br * size; i <= r; i++) count[a[i]]++;

            for (int v = 1; v <= 500 && v <= x; v++) x += count[v] + bit[v].range_sum_query(bl + 1, br);
            cout << x << "\n";
        }
    }
}
