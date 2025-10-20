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

    int A, B, T, t_a, t_b, C;
    cin >> A >> B >> T >> t_a >> t_b >> C;

    vector<pair<int, int>> skills(C);
    vector<int> compress(C);
    for (int i = 0; i < C; i++) {
        auto &[a, b] = skills[i];
        cin >> a >> b;

        compress[i] = skills[i].second;
    }
    sort(compress.begin(), compress.end());
    compress.erase(unique(compress.begin(), compress.end()), compress.end());
    for (auto &[a, b] : skills) b = lower_bound(compress.begin(), compress.end(), b) - compress.begin();
    sort(skills.begin(), skills.end());

    vector<int> valid;
    for (auto [a, b] : skills)
        if (a > A && (valid.empty() || a != valid.back())) valid.emplace_back(a);

    FenwickTree<int> fw(compress.size() + 1);
    auto count = [&](int b) {
        int i = upper_bound(compress.begin(), compress.end(), b) - compress.begin();
        return !i ? 0 : fw.pref_sum(i);
    };

    int offers = 0, i = 0;
    for (; i < C && skills[i].first <= A; i++) fw.update(skills[i].second + 1, 1);
    offers = max(offers, count(B + T / t_b));

    for (int a : valid) {
        int t = T - (a - A) * t_a;
        if (t < 0) break;
        for (; i < C && skills[i].first <= a; i++) fw.update(skills[i].second + 1, 1);
        offers = max(offers, count(B + t / t_b));
    }
    cout << offers;
}
