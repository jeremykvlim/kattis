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

    int n, Q, R;
    cin >> n >> Q >> R;

    vector<array<int, 3>> points(n);
    vector<int> zs(n);
    for (int i = 0; i < n; i++) {
        cin >> points[i][0] >> points[i][1] >> points[i][2];

        zs[i] = points[i][2];
    }
    sort(zs.begin(), zs.end());
    zs.erase(unique(zs.begin(), zs.end()), zs.end());

    vector<array<int, 4>> sweep;
    for (auto [x, y, z] : points) sweep.push_back({x, y, (int) (lower_bound(zs.begin(), zs.end(), z) - zs.begin()) + 1, 0});

    for (int q = 0; q < Q; q++) {
        int x1, y1, z1, x2, y2, z2;
        cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;

        x1 -= R;
        y1 -= R;
        z1 -= R;
        x2 += R;
        y2 += R;
        z2 += R;

        for (auto mask = 0U; mask < 8; mask++) sweep.push_back({mask & 1 ? x1 - 1 : x2,
                                                                mask & 2 ? y1 - 1 : y2,
                                                                (int) (upper_bound(zs.begin(), zs.end(), mask & 4 ? z1 - 1 : z2) - zs.begin()),
                                                                (popcount(mask) & 1) ? -q - 1 : q + 1});

    }
    sort(sweep.begin(), sweep.end(), [&](const auto &a1, const auto &a2) { return a1[0] != a2[0] ? a1[0] < a2[0] : !a1[3] > !a2[3]; });

    auto temp = sweep;
    vector<int> count(Q);
    FenwickTree<int> fw(zs.size() + 1);
    auto dnc = [&](auto &&self, int l, int r, auto &from, auto &to) -> void {
        if (l + 1 >= r) {
            if (l + 1 == r) to[l] = from[l];
            return;
        }

        int m = l + (r - l) / 2;
        self(self, l, m, to, from);
        self(self, m, r, to, from);

        int i = l, j = l, k = m;
        for (; k < r; j++, k++) {
            for (; i < m && from[i][1] <= from[k][1]; i++, j++) {
                if (!from[i][3]) fw.update(from[i][2], 1);
                to[j] = from[i];
            }

            if (from[k][3]) {
                int q = from[k][3], c = fw.pref_sum(from[k][2]);
                if (q > 0) count[q - 1] += c;
                else count[-q - 1] -= c;
            }
            to[j] = from[k];
        }

        for (; l < i; l++)
            if (!from[l][3]) fw.update(from[l][2], -1);

        for (; i < m; i++, j++) to[j] = from[i];
    };
    dnc(dnc, 0, sweep.size(), sweep, temp);
    for (int c : count) cout << c << "\n";
}
