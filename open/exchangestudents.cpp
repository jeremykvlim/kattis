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

    vector<int> g(n + 1), h(n + 1);
    for (int i = 1; i <= n; i++) cin >> g[i];
    for (int i = 1; i <= n; i++) cin >> h[i];

    FenwickTree<int> fw_g(n + 1), fw_h(n + 1);
    for (int i = 1; i <= n; i++) {
        fw_g.update(i, 1);
        fw_h.update(i, 1);
    }

    vector<int> ranks_g(n + 1), ranks_h(n + 1);
    iota(ranks_g.begin(), ranks_g.end(), 0);
    iota(ranks_h.begin(), ranks_h.end(), 0);
    stable_sort(ranks_g.begin() + 1, ranks_g.end(), [&](int i, int j) { return g[i] < g[j]; });
    stable_sort(ranks_h.begin() + 1, ranks_h.end(), [&](int i, int j) { return h[i] < h[j]; });

    vector<int> indices(n + 1);
    for (int i = 1; i <= n; i++) indices[ranks_g[i]] = i;

    auto s = 0LL;
    for (int i = 1; i <= n; i++) {
        s += abs(fw_g.pref_sum(ranks_g[i]) - fw_h.pref_sum(ranks_h[i]));
        fw_g.update(ranks_g[i], -1);
        fw_h.update(ranks_h[i], -1);
    }
    cout << s << "\n";

    s = min(s, (long long) 2e5);

    list<int> positions(n + 1);
    iota(positions.begin(), positions.end(), 0);

    vector<list<int>::iterator> its(n + 1);
    its[n] = prev(positions.end());
    for (int i = n - 1; i; i--) its[i] = prev(its[i + 1]);

    for (int k = 1; k <= n; k++) {
        int gi = ranks_g[k], hi = ranks_h[k];

        auto exchange = [&](auto l) {
            while (*l != hi) {
                auto r = next(l);
                while (g[*l] == g[*r]) r++;
                while (l != r) {
                    int i = *prev(r), j = *r;
                    cout << i << " " << j << "\n";

                    swap(g[i], g[j]);
                    swap(indices[i], indices[j]);
                    swap(ranks_g[indices[i]], ranks_g[indices[j]]);
                    if (!--s) exit(0);
                    r--;
                }
                l++;
            }
        };

        if (gi < hi) exchange(its[gi]);
        else exchange(make_reverse_iterator(next(its[gi])));
        
        positions.erase(its[hi]);
    }
}
