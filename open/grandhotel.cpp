#include <bits/stdc++.h>
using namespace std;

struct Hash {
    template <typename T>
    static inline void combine(size_t &h, const T &v) {
        h ^= Hash{}(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }

    template <typename T>
    size_t operator()(const T &v) const {
        if constexpr (requires { tuple_size<T>::value; })
            return apply([](const auto &...e) {
                size_t h = 0;
                (combine(h, e), ...);
                return h;
            }, v);
        else if constexpr (requires { declval<T>().begin(); declval<T>().end(); } && !is_same_v<T, string>) {
            size_t h = 0;
            for (const auto &e : v) combine(h, e);
            return h;
        } else return hash<T>{}(v);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<array<int, 4>> R(n);
    for (auto &[x1, x2, x3, x4] : R) cin >> x1 >> x2 >> x3 >> x4;

    auto encode = [&](const array<int, 4> &p) {
        return (p[1] % 3 * 3 + p[2] % 3) * 3 + p[3] % 3;
    };

    vector<int> count(27, 0);
    for (auto p : R) count[encode(p)]++;
    int a = min_element(count.begin(), count.end()) - count.begin();
    array<int, 3> residue{a / 9, a / 3 % 3, a % 3};

    vector<array<int, 3>> normalized(n);
    unordered_map<array<int, 3>, array<int, 2>, Hash> intervals;
    for (int i = 0; i < n; i++) {
        for (int d = 0; d < 3; d++) {
            int delta = (residue[d] - R[i][d + 1] % 3 + 4) % 3 - 1;
            normalized[i][d] = R[i][d + 1] + delta;
            if (!(0 <= normalized[i][d] && normalized[i][d] <= 399)) normalized[i][d] -= 3 * delta;
        }

        auto &[l, r] = intervals.try_emplace(normalized[i], array{R[i][0], R[i][0]}).first->second;
        l = min(l, R[i][0]);
        r = max(r, R[i][0]);
    }

    vector<bool> valid(400, true);
    for (int i = 0; i < n; i++)
        for (int r = encode(R[i]) == a ? 3 : 1, x = max(0, R[i][0] - r); x <= min(399, R[i][0] + r); x++) valid[x] = false;
    int x0 = find(valid.begin(), valid.end(), true) - valid.begin();

    if (intervals.size() > 1)
        for (auto &[_, s] : intervals) {
            auto &[l, r] = s;
            l = min(l, x0);
            r = max(r, x0);
        }

    unordered_set<array<int, 4>, Hash> H;
    auto add = [&](auto &cell, int d, int x) {
        H.emplace(cell);
        while (cell[d] != x) {
            cell[d] += cell[d] < x ? 1 : -1;
            H.emplace(cell);
        }
    };

    for (auto [p, s] : intervals) {
        array<int, 4> cell{s[0], p[0], p[1], p[2]};
        add(cell, 0, s[1]);
    }

    for (int i = 0; i < n; i++)
        if (encode(R[i]) == a) {
            auto [l, r] = intervals[normalized[i]];
            int x = R[i][0];
            if (l < x && x < r) {
                H.erase({x + 1, normalized[i][0], normalized[i][1], normalized[i][2]});
                array<int, 4> cell{x - 1, normalized[i][0], normalized[i][1], normalized[i][2]};
                add(cell, 1, cell[1] + (cell[1] < 399 ? 1 : -1));
                add(cell, 2, cell[2] + (cell[2] < 399 ? 1 : -1));
                add(cell, 0, x + 2);
                add(cell, 2, normalized[i][1]);
                add(cell, 1, normalized[i][0]);
            }
        }

    if (intervals.size() > 1) {
        auto root = intervals.begin()->first;
        for (auto [p, s] : intervals) {
            array<int, 4> cell{x0, p[0], p[1], p[2]};
            for (int d = 1; d < 4; d++) add(cell, d, root[d - 1]);
        }
    }

    for (int i = 0; i < n; i++)
        for (int d = 1; d < 4; d++) add(R[i], d, normalized[i][d - 1]);

    cout << H.size() << "\n";
    for (auto [x1, x2, x3, x4] : H) cout << x1 << " " << x2 << " " << x3 << " " << x4 << "\n";
}