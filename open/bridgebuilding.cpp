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

    int n, m;
    cin >> n >> m;

    vector<int> h(n + 1);
    for (int i = 1; i <= n; i++) cin >> h[i];

    vector<long long> pref(n + 1, 0);
    for (int i = 1; i < n; i++) pref[i + 1] = pref[i] + abs(h[i] - h[i + 1]) + 1;

    auto base = 0LL;
    vector<pair<int, int>> pairs(m);
    vector<vector<int>> as(n + 1), bs(n + 1);
    for (auto &[a, b] : pairs) {
        cin >> a >> b;

        if (a > b) swap(a, b);
        as[b].emplace_back(a);
        bs[a].emplace_back(b);
        base += pref[b] - pref[a];
    }

    vector<int> left, right;
    vector<vector<int>> indices_l(n + 1), indices_r(n + 1);
    stack<int> mono;
    for (int i = 1; i <= n; i++) {
        while (!mono.empty() && h[mono.top()] < h[i]) mono.pop();
        if (!mono.empty() && h[mono.top()] == h[i]) {
            int l = mono.top(), r = i;
            left.emplace_back(l);
            right.emplace_back(r);
            indices_l[l].emplace_back(left.size() - 1);
            indices_r[r].emplace_back(right.size() - 1);
        }
        mono.emplace(i);
    }

    int k = left.size();
    vector<long long> total(k);
    FenwickTree<int> count1(n + 1), count2(n + 1);
    FenwickTree<long long> sum1(n + 1), sum2(n + 1);
    for (int i = 1; i <= n; i++) {
        for (int b : bs[i]) {
            count1.update(b, 1);
            sum1.update(b, pref[b]);
        }

        for (int id : indices_l[i]) {
            int j = right[id];

            auto target = pref[j] + pref[i] + j - i;
            int l = max(i + 1, (int) (upper_bound(pref.begin(), pref.end(), target / 2) - pref.begin())), r = j - 1;
            if (l <= r) total[id] += 2 * sum1.range_sum_query(l - 1, r) - target * count1.range_sum_query(l - 1, r);
        }
    }

    auto most = 0LL;
    for (int j = n; j; j--) {
        for (int a : as[j]) {
            count2.update(a, 1);
            sum2.update(a, pref[a]);
        }

        for (int id : indices_r[j]) {
            int i = left[id];

            auto target = pref[j] + pref[i] - j + i;
            int l = i + 1, r = min(j - 1, (int) (lower_bound(pref.begin(), pref.end(), (target + 1) / 2) - pref.begin() - 1));
            if (l <= r) total[id] -= 2 * sum2.range_sum_query(l - 1, r) - target * count2.range_sum_query(l - 1, r);
            most = max(most, total[id] += count2.pref_sum(i) * (pref[j] - pref[i] - j + i));
        }
    }

    vector<long long> target(k);
    vector<array<long long, 6>> sweep;
    for (auto [a, b] : pairs) sweep.push_back({2 * (pref[b] - pref[a]), a, b, 0, -1, 0});
    for (int id = 0; id < k; id++) {
        int i = left[id], j = right[id];
        target[id] = pref[j] - pref[i] + j - i;

        if (i + 1 < j) {
            sweep.push_back({target[id], n, j - 1, 1, id, 1});
            sweep.push_back({target[id], i, j - 1, 1, id, -1});
        }
    }
    sort(sweep.begin(), sweep.end(), [&](const auto &a1, const auto &a2) { return a1[0] != a2[0] ? a1[0] > a2[0] : a1[3] > a2[3]; });

    FenwickTree<int> count(n + 1);
    FenwickTree<long long> sum(n + 1);
    auto dnc = [&](auto &&self, int l, int r) -> void {
        if (l + 1 >= r) return;

        int m = l + (r - l) / 2;
        self(self, l, m);
        self(self, m, r);

        int j = l;
        for (int i = m; i < r; i++) {
            for (; j < m && sweep[i][1] >= sweep[j][1]; j++)
                if (!sweep[j][3]) {
                    count.update(sweep[j][2], 1);
                    sum.update(sweep[j][2], sweep[j][0]);
                }

            if (sweep[i][3]) total[sweep[i][4]] += sweep[i][5] * (sum.pref_sum(sweep[i][2]) - target[sweep[i][4]] * count.pref_sum(sweep[i][2]));
        }

        for (int i = l; i < j; i++)
            if (!sweep[i][3]) {
                count.update(sweep[i][2], -1);
                sum.update(sweep[i][2], -sweep[i][0]);
            }

        inplace_merge(sweep.begin() + l, sweep.begin() + m, sweep.begin() + r, [&](const auto &a1, const auto &a2) { return a1[1] < a2[1]; });
    };
    dnc(dnc, 0, sweep.size());

    for (auto s : total) most = max(most, s);
    cout << base - most;
}
