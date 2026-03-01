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

    int lower_bound(T k) {
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

    int n, Q;
    cin >> n >> Q;
    cin.ignore();

    vector<string> titles(n);
    for (auto &t : titles) {
        getline(cin, t);
        for (char &c : t)
            if (isupper(c)) c = tolower(c);
    }
    auto temp = titles;
    sort(temp.begin(), temp.end());
    temp.erase(unique(temp.begin(), temp.end()), temp.end());

    vector<int> rank(n);
    for (int i = 0; i < n; i++) rank[i] = lower_bound(temp.begin(), temp.end(), titles[i]) - temp.begin() + 1;

    vector<int> r(n);
    FenwickTree<int> fw1(n + 1);
    for (int i = n - 1; ~i; i--) {
        r[i] = rank[i] - fw1.pref_sum(rank[i]);
        fw1.update(rank[i], 1);
    }

    vector<long long> diff(n + 1, 0), count(n + 1, 0), pref(n + 1, 0);
    for (int i = 0; i < n; i++) {
        diff[r[i]]++;
        diff[i + 1]--;
    }
    for (int i = 1; i <= n; i++) {
        count[i] = count[i - 1] + diff[i];
        pref[i] = pref[i - 1] + count[i];
    }

    vector<long long> answers(Q, -1);
    vector<vector<pair<long long, int>>> fatigues(n + 1);
    for (int q = 0; q < Q; q++) {
        long long f;
        cin >> f;

        if (f <= pref[n]) {
            int i = lower_bound(pref.begin() + 1, pref.end(), f) - pref.begin();
            fatigues[i].emplace_back(f - pref[i - 1], q);
        }
    }

    vector<vector<int>> indices(n + 1);
    for (int i = 1; i <= n; i++) indices[r[i - 1]].emplace_back(i);

    auto total = 0LL;
    FenwickTree<int> pos(n + 1);
    for (auto i : indices[0]) pos.update(i, 1);
    for (int i = 1; i <= n; i++) {
        for (auto j : indices[i]) pos.update(j, 1);
        for (auto [f, q] : fatigues[i]) answers[q] = total + pos.lower_bound(pos.pref_sum(i) + f) - i;
        total += n - i;
    }

    for (auto a : answers)
        if (!~a) cout << "IMPOSSIBLE\n";
        else cout << a << "\n";
}
