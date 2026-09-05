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

    int n, q;
    cin >> n >> q;

    vector<pair<int, int>> x(n);
    for (int i = 1; i <= n; i++) {
        cin >> x[i - 1].first;

        x[i - 1].second = i;
    }
    sort(x.begin(), x.end());

    vector<int> rank(n + 1), pos(n + 1);
    for (int i = 1; i <= n; i++) {
        rank[x[i - 1].second] = i;
        pos[i] = x[i - 1].second;
    }

    vector<int> pref1(n + 1, 0), pref2(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        pref1[i] = rank[i] == i ? pref1[i - 1] : i;
        pref2[i] = pref2[i - 1] + (rank[i] > i) - (pos[i] < i);
    }

    vector<array<int, 2>> queries(q);
    vector<vector<pair<int, int>>> sweep(n + 1);
    for (int i = 0; i < q; i++) {
        int a, b;
        cin >> a >> b;

        b = n - b;
        queries[i] = {a, b};
        if (!pref1[n]) queries[i] = {0, 0};
        else if (!pref1[b] || pref1[n] <= a) queries[i] = {0, 1};
        else if (a <= b) queries[i] = {0, pref1[b] <= a && !pref2[a] && !pref2[b] ? 2 : -1};
        else {
            sweep[a].emplace_back(b, i << 1);
            sweep[b].emplace_back(a, i << 1 | 1);
        }
    }

    FenwickTree<int> fw(n + 1);
    vector<int> count(2 * q);
    for (int a = 0; a <= n; a++) {
        if (a) fw.update(rank[a], 1);
        for (auto [b, i] : sweep[a]) count[i] = fw.pref_sum(b);
    }

    for (int i = 0; i < q; i++) {
        auto [a, b] = queries[i];
        if (!a) {
            cout << b << "\n";
            continue;
        }

        int d = a - b;
        auto ops = [&](int c1, int c2) {
            return max((c1 + d - 1) / d * 2, (c2 + d - 1) / d * 2 + 1);
        };
        cout << max(2, min(ops(pref2[a], b - count[i << 1]), ops(pref2[b], b - count[i << 1 | 1]))) << "\n";
    }
}
