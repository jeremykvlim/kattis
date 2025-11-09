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

    int n;
    cin >> n;

    auto total = (long long) n * (n - 1) / 2;
    array<vector<array<int, 3>>, 2> h, v;
    vector<pair<int, int>> h_ranges, v_ranges;
    while (n--) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        x1 *= 2;
        y1 *= 2;
        x2 *= 2;
        y2 *= 2;

        int dx = abs(x1 - x2), dy = abs(y1 - y2);
        if (dx < dy) {
            int ym = (y1 + y2) / 2, yl = ym - dx / 2, yr = ym + dx / 2;
            if (x1 > x2) {
                swap(x1, x2);
                swap(y1, y2);
            }
            if (y1 > y2) h[0].push_back({yl, yr, yl - x1});
            else h[1].push_back({yl, yr, yl + x2});
            h_ranges.emplace_back(yl, yr);
        } else if (dx > dy) {
            int xm = (x1 + x2) / 2, xl = xm - dy / 2, xr = xm + dy / 2;
            if (y1 > y2) {
                swap(x1, x2);
                swap(y1, y2);
            }
            if (x1 > x2) v[0].push_back({xl, xr, xl - y1});
            else v[1].push_back({xl, xr, xl + y2});
            v_ranges.emplace_back(xl, xr);
        }
    }

    auto count = [&](const auto &axis, const auto &ranges) -> long long {
        if (ranges.empty()) return 0;

        vector<int> sweep;
        for (auto [l, r] : ranges) sweep.emplace_back(r);
        sort(sweep.begin(), sweep.end());

        vector<int> order(ranges.size());
        iota(order.begin(), order.end(), 0);
        sort(order.begin(), order.end(), [&](int i, int j) { return ranges[i].first < ranges[j].first; });

        int p = 0;
        auto c = 0LL;
        for (int i : order) {
            for (; p < sweep.size() && sweep[p] < ranges[i].first; p++);
            c += p;
        }

        for (int d = 0; d < 2; d++) {
            if (axis[d].empty()) continue;

            vector<int> deduped;
            for (auto [l, r, k] : axis[d]) deduped.emplace_back(k);
            sort(deduped.begin(), deduped.end());
            deduped.erase(unique(deduped.begin(), deduped.end()), deduped.end());

            vector<array<int, 3>> compressed;
            for (auto [l, r, k] : axis[d]) compressed.push_back({l, r, (int) (lower_bound(deduped.begin(), deduped.end(), k) - deduped.begin())});
            sort(compressed.begin(), compressed.end());

            int K = 0;
            for (auto &e : compressed) K = max(K, e[2] + 1);
            FenwickTree<long long> fw(K + 1);

            auto dnc = [&](auto &&self, int l, int r) -> long long {
                if (l + 1 >= r) return 0;

                int ll = compressed[l][0], lr = compressed[r - 1][0];
                if (ll == lr) return 0;

                int left = l, right = r, mid, lm = ll + (lr - ll) / 2;
                while (left < right) {
                    mid = (left + right) >> 1;

                    if (compressed[mid][0] <= lm) left = mid + 1;
                    else right = mid;
                }
                int m = left;
                auto sum = self(self, l, m) + self(self, m, r);

                vector<int> indices_l(m - l);
                iota(indices_l.begin(), indices_l.end(), l);
                sort(indices_l.begin(), indices_l.end(), [&](int i, int j) { return compressed[i][1] < compressed[j][1]; });

                vector<int> indices_rl(r - m);
                iota(indices_rl.begin(), indices_rl.end(), m);
                sort(indices_rl.begin(), indices_rl.end(), [&](int i, int j) { return compressed[i][0] < compressed[j][0]; });

                vector<int> indices_rr(r - m);
                iota(indices_rr.begin(), indices_rr.end(), m);
                sort(indices_rr.begin(), indices_rr.end(), [&](int i, int j) { return compressed[i][1] < compressed[j][1]; });
                auto process = [&](const auto &indices_r, bool r) {
                    int p = 0;
                    auto s = 0LL;
                    for (int i : indices_r) {
                        for (; p < indices_l.size() && compressed[indices_l[p]][1] < compressed[i][r]; p++) fw.update(compressed[indices_l[p]][2] + 1, 1);
                        s += fw.pref_sum(compressed[i][2]);
                    }
                    while (p) fw.update(compressed[indices_l[--p]][2] + 1, -1);
                    return s;
                };
                sum += process(indices_rr, true) - process(indices_rl, false);

                inplace_merge(compressed.begin() + l, compressed.begin() + m, compressed.begin() + r);
                return sum;
            };
            c += dnc(dnc, 0, compressed.size());
        }
        return c;
    };
    cout << total - count(h, h_ranges) - count(v, v_ranges);
}
