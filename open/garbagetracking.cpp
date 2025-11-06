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

    int r, q;
    cin >> r >> q;

    vector<array<int, 4>> h_walks, v_walks;
    while (r--) {
        int x, y, w, h, c;
        cin >> x >> y >> w >> h >> c;

        if (w) {
            h_walks.push_back({y, x, x + w - 1, c});
            h_walks.push_back({y + h, x, x + w - 1, c});
        }
        if (h) {
            v_walks.push_back({x, y, y + h - 1, c});
            v_walks.push_back({x + w, y, y + h - 1, c});
        }
    }

    vector<array<int, 5>> h_queries, v_queries;
    for (int i = 0; i < q; i++) {
        int x, y, w, h;
        cin >> x >> y >> w >> h;

        if (w) {
            h_queries.push_back({y + h, x, x + w - 1, i, 1});
            h_queries.push_back({y - 1, x, x + w - 1, i, -1});
        }
        if (h) {
            v_queries.push_back({x + w, y, y + h - 1, i, 1});
            v_queries.push_back({x - 1, y, y + h - 1, i, -1});
        }
    }

    FenwickTree<long long> fw_x1(4503), fw_x2(4503), fw_y1(4503), fw_y2(4503);
    auto offset = [&](int z) {
        return z + 2251;
    };

    auto sum = [&](auto &fw1, auto &fw2, int l, int r) -> long long {
        if (l > r) return 0;
        auto pref_sum = [&](int i) {
            return fw1.pref_sum(i) * i - fw2.pref_sum(i);
        };
        return pref_sum(r) - pref_sum(l - 1);
    };

    auto update = [&](auto &fw1, auto &fw2, int l, int r, long long v) {
        if (l > r) return;
        fw1.update(l, v);
        fw1.update(r + 1, -v);
        fw2.update(l, v * (l - 1));
        fw2.update(r + 1, -v * r);
    };

    sort(h_walks.begin(), h_walks.end());
    sort(h_queries.begin(), h_queries.end());
    vector<long long> garbage(q, 0);
    for (int w = 0; auto [y, x1, x2, i, sgn] : h_queries) {
        for (; w < h_walks.size() && h_walks[w][0] <= y; w++) update(fw_x1, fw_x2, offset(h_walks[w][1]), offset(h_walks[w][2]), h_walks[w][3]);
        garbage[i] += sgn * sum(fw_x1, fw_x2, offset(x1), offset(x2));
    }

    sort(v_walks.begin(), v_walks.end());
    sort(v_queries.begin(), v_queries.end());
    for (int w = 0; auto [x, y1, y2, i, sgn] : v_queries) {
        for (; w < v_walks.size() && v_walks[w][0] <= x; w++) update(fw_y1, fw_y2, offset(v_walks[w][1]), offset(v_walks[w][2]), v_walks[w][3]);
        garbage[i] += sgn * sum(fw_y1, fw_y2, offset(y1), offset(y2));
    }

    for (auto g : garbage) cout << g << "\n";
}
