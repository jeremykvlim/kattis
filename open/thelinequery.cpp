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

    int n, k, q;
    cin >> n >> k >> q;

    vector<pair<int, int>> pairs(k);
    vector<int> ls(k), rs(k);
    auto total = 0LL;
    for (int i = 0; i < k; i++) {
        int s, t;
        cin >> s >> t;

        auto &[l, r] = pairs[i];
        tie(l, r) = minmax(s, t);
        ls[i] = l;
        rs[i] = r;
        total += r - l;
    }
    sort(pairs.begin(), pairs.end());
    sort(ls.begin(), ls.end());
    ls.erase(unique(ls.begin(), ls.end()), ls.end());
    sort(rs.begin(), rs.end());
    rs.erase(unique(rs.begin(), rs.end()), rs.end());

    vector<pair<int, int>> queries(q);
    for (auto &[i, j] : queries) {
        cin >> i >> j;

        if (i > j) swap(i, j);
    }

    vector<int> order(q);
    iota(order.begin(), order.end(), 0);

    int sr = rs.size();
    FenwickTree<int> count_r(sr + 1);
    FenwickTree<long long> sum_r(sr + 1);
    sort(order.begin(), order.end(), [&](int i, int j) { return queries[i].first < queries[j].first; });

    auto update_r = [&](int r) {
        int p = lower_bound(rs.begin(), rs.end(), r) - rs.begin() + 1;
        count_r.update(p, 1);
        sum_r.update(p, r);
    };

    auto query_r = [&](int r) -> pair<int, long long> {
        int p = upper_bound(rs.begin(), rs.end(), r) - rs.begin();
        return {count_r.pref_sum(p), sum_r.pref_sum(p)};
    };

    vector<long long> saved(q, 0);
    int mid = 0;
    for (int qi : order) {
        auto [l1, r1] = queries[qi];
        for (; mid < k && pairs[mid].first <= l1; mid++) update_r(pairs[mid].second);

        auto [c1, s1] = query_r(r1);
        auto [c2, s2] = query_r((l1 + r1 + 1) / 2);
        saved[qi] += 2 * (s1 - s2) - (long long) (l1 + r1 + 1) * (c1 - c2);
        if (l1 + 1 < r1) saved[qi] += (long long) (r1 - l1 - 1) * (mid - c1);
    }

    int sl = ls.size();
    FenwickTree<int> count_l(sl + 1);
    FenwickTree<long long> sum_l(sl + 1);
    sort(order.begin(), order.end(), [&](int i, int j) { return queries[i].second > queries[j].second; });

    auto update_l = [&](int l) {
        int p = lower_bound(ls.begin(), ls.end(), l) - ls.begin() + 1;
        count_l.update(p, 1);
        sum_l.update(p, l);
    };

    auto query_l = [&](int l) -> pair<int, long long> {
        int p = upper_bound(ls.begin(), ls.end(), l) - ls.begin();
        return {count_l.pref_sum(p), sum_l.pref_sum(p)};
    };

    vector<int> people(k);
    iota(people.begin(), people.end(), 0);
    sort(people.begin(), people.end(), [&](int i, int j) { return pairs[i].second > pairs[j].second; });

    mid = 0;
    for (int qi : order) {
        auto [l1, r1] = queries[qi];
        for (; mid < k && pairs[people[mid]].second > r1; mid++) update_l(pairs[people[mid]].first);

        if (l1 < (l1 + r1 - 2) / 2) {
            auto [c1, s1] = query_l((l1 + r1 - 2) / 2);
            auto [c2, s2] = query_l(l1);
            saved[qi] += (long long) (l1 + r1 - 1) * (c1 - c2) - 2 * (s1 - s2);
        }
    }

    vector<int> diffs(k);
    for (int i = 0; i < k; i++) diffs[i] = pairs[i].first - pairs[i].second;
    sort(diffs.begin(), diffs.end());
    diffs.erase(unique(diffs.begin(), diffs.end()), diffs.end());

    vector<array<int, 6>> sweep;
    for (auto [l, r] : pairs) {
        int d = r - l;
        sweep.push_back({-l, r, (int) (lower_bound(diffs.begin(), diffs.end(), -d) - diffs.begin()) + 1, 0, -1, d});
    }

    for (int i = 0; i < q; i++) {
        auto [l, r] = queries[i];
        int d = r - l;
        sweep.push_back({-l - 1, r, (int) (upper_bound(diffs.begin(), diffs.end(), -(d + 3) / 2) - diffs.begin()), 1, i, d + 1});
    }
    sort(sweep.begin(), sweep.end(), [&](const auto &a1, const auto &a2) { return a1[0] != a2[0] ? a1[0] < a2[0] : a1[3] < a2[3]; });

    FenwickTree<int> count_d(diffs.size() + 1);
    FenwickTree<long long> sum_d(diffs.size() + 1);
    auto dnc = [&](auto &&self, int l, int r) -> void {
        if (l + 1 >= r) return;

        int m = l + (r - l) / 2;
        self(self, l, m);
        self(self, m, r);

        int j = l;
        for (int i = m; i < r; i++) {
            for (; j < m && sweep[i][1] >= sweep[j][1]; j++)
                if (!sweep[j][3]) {
                    count_d.update(sweep[j][2], 1);
                    sum_d.update(sweep[j][2], sweep[j][5]);
                }
            if (sweep[i][3]) saved[sweep[i][4]] += 2 * sum_d.pref_sum(sweep[i][2]) - (long long) sweep[i][5] * count_d.pref_sum(sweep[i][2]);
        }

        for (int i = l; i < j; i++)
            if (!sweep[i][3]) {
                count_d.update(sweep[i][2], -1);
                sum_d.update(sweep[i][2], -sweep[i][5]);
            }

        inplace_merge(sweep.begin() + l, sweep.begin() + m, sweep.begin() + r, [&](const auto &a1, const auto &a2) { return a1[1] < a2[1]; });
    };
    dnc(dnc, 0, sweep.size());
    for (auto v : saved) cout << total - v << "\n";
}
