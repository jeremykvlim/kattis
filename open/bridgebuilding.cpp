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
        if (l > r) return 0;
        return pref_sum(r) - pref_sum(l - 1);
    }

    FenwickTree(int n) : BIT(n, 0) {}
};

template <typename T>
struct FenwickTree2D {
    vector<vector<T>> BIT;
    vector<vector<int>> js;

    FenwickTree2D(int n, const vector<vector<int>> &js) : BIT(n), js(js) {
        for (int i = 0; i < n; i++) BIT[i].assign(js[i].size() + 1, 0);
    }

    void update(int i, int j, T v) {
        for (int temp = j; i < BIT.size(); i += i & -i, j = temp)
            for (j = upper_bound(js[i].begin(), js[i].end(), j) - js[i].begin(); j < BIT[i].size(); j += j & -j) BIT[i][j] += v;
    }

    T pref_sum(int i, int j) {
        T sum = 0;
        for (int temp = j; i; i -= i & -i, j = temp)
            for (j = upper_bound(js[i].begin(), js[i].end(), j) - js[i].begin(); j; j -= j & -j) sum += BIT[i][j];
        return sum;
    }

    T rectangle_sum(int xl, int yl, int xr, int yr) {
        if (xl > xr || yl > yr) return 0;
        return pref_sum(xr, yr) - pref_sum(xl - 1, yr) - pref_sum(xr, yl - 1) + pref_sum(xl - 1, yl - 1);
    }
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
    vector<vector<int>> as(n + 1), bs(n + 1), js(n + 1);
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;

        if (a > b) swap(a, b);
        pairs[i] = {a, b};
        as[b].emplace_back(a);
        bs[a].emplace_back(b);
        base += pref[b] - pref[a];
        for (; a <= n; a += a & -a) js[a].emplace_back(b);
    }

    auto calc = [&](auto &p) {
        return 2 * (pref[p.second] - pref[p.first]);
    };
    sort(pairs.begin(), pairs.end(), [&](const auto &p1, const auto &p2) { return calc(p1) > calc(p2); });

    for (int i = 1; i <= n; i++) {
        sort(js[i].begin(), js[i].end());
        js[i].erase(unique(js[i].begin(), js[i].end()), js[i].end());
    }

    int count = 0;
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
            count++;
        }
        mono.emplace(i);
    }

    vector<long long> total(count, 0);
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
            if (l <= r) total[id] += 2 * sum1.range_sum_query(l, r) - target * count1.range_sum_query(l, r);
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
            if (l <= r) total[id] -= 2 * sum2.range_sum_query(l, r) - target * count2.range_sum_query(l, r);
            most = max(most, total[id] += count2.pref_sum(i) * (pref[j] - pref[i] - j + i));
        }
    }

    FenwickTree2D<long long> count3(n + 1, js), sum3(n + 1, js);
    vector<long long> target(count, 0);
    for (int id = 0; id < count; id++) {
        int i = left[id], j = right[id];
        target[id] = pref[j] - pref[i] + j - i;
    }

    vector<int> order(count);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int i, int j) { return target[i] > target[j]; });
    for (int k = 0; int id : order) {
        int i = left[id], j = right[id];
        for (; k < pairs.size() && calc(pairs[k]) > target[id]; k++) {
            auto [a, b] = pairs[k];
            count3.update(a, b, 1);
            sum3.update(a, b, calc(pairs[k]));
        }

        if (i + 1 < j) {
            int l = i + 1, r = j - 1;
            total[id] += sum3.rectangle_sum(l, l, r, r) - target[id] * count3.rectangle_sum(l, l, r, r);
        }
        most = max(most, total[id]);
    }
    cout << base - most;
}
