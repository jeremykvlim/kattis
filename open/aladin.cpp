#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct FenwickTree {
    vector<T> BIT;

    void update(int i, T value) {
        for (; i < BIT.size(); i += i & -i) BIT[i] += value;
    }

    T pref_sum(int i) {
        T sum = 0;
        for (; i; i &= (i - 1)) sum += BIT[i];
        return sum;
    }

    FenwickTree(int n) : BIT(n, 0) {}
};

array<long long, 3> operator+(array<long long, 3> t1, array<long long, 3> t2) {
    auto [x1, y1, z1] = t1;
    auto [x2, y2, z2] = t2;
    return {x1 + x2, y1 + y2, x1 * y2 + z1 + z2};
}

array<long long, 3> operator*(array<long long, 3> t, long long v) {
    auto [x, y, z] = t;
    return {v * x, v * y, (v * (v - 1) / 2) * x * y  + v * z};
}

array<long long, 3> calc(long long range, long long a, long long x, long long b, array<long long, 3> t1, array<long long, 3> t2) {
    if (!range) return {0, 0, 0};
    if (x >= b) return calc(range, a, x % b, b, t1, t2) + t1 * (x / b);
    if (a >= b) return calc(range, a % b, x, b, t2 * (a / b) + t1, t2);

    auto r = (a * range + x) / b;
    if (!r) return t1 * range;

    return t1 * ((b - x - 1) / a) + t2 + calc(r - 1, b, (b - x - 1) % a, a, t2, t1) + t1 * (range - (b * r - x - 1) / a);
}

long long sum(int l, int r, int a, int b) {
    int x = ((long long) a * (l - 1)) % b;
    auto [_, y, z] = calc(r - l + 1, a, x, b, {a, 1, a}, {-b, 0, 0});
    return x * y + z;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    vector<int> boxes;
    vector<array<int, 4>> queries(q);
    for (auto &[l, r, a, b] : queries) {
        int c;
        cin >> c >> l >> r;

        if (c == 1) cin >> a >> b;
        else {
            boxes.emplace_back(l - 1);
            boxes.emplace_back(r);
        }
    }
    sort(boxes.begin(), boxes.end());
    boxes.erase(unique(boxes.begin(), boxes.end()), boxes.end());

    auto index = [&](int v) -> int {
        return lower_bound(boxes.begin(), boxes.end(), v) - boxes.begin() + 1;
    };

    FenwickTree<long long> fw(boxes.size() + 1);
    auto cmp = [](auto t1, auto t2) {return get<1>(t1) < get<1>(t2);};
    set<array<int, 6>, decltype(cmp)> s(cmp);
    s.insert({1, n, 0, 1, 1, n});
    for (auto [l1, r1, a1, b1] : queries)
        if (a1 && b1) {
            for (auto it = s.lower_bound({0, l1, 0, 0, 0, 0}); it != s.end(); it = s.lower_bound({0, l1, 0, 0, 0, 0})) {
                auto [l2, r2, a2, b2, l3, r3] = *it;
                if (l2 > r1) break;

                s.erase(it);
                fw.update(index(r2), -sum(l3, r3, a2, b2));
                if (l2 < l1) {
                    s.insert({l2, l1 - 1, a2, b2, l3, l1 - l2 + l3 - 1});
                    fw.update(index(l1 - 1), sum(l3, l1 - l2 + l3 - 1, a2, b2));
                }
                if (r1 < r2) {
                    s.insert({r1 + 1, r2, a2, b2, r1 - r2 + r3 + 1, r3});
                    fw.update(index(r2), sum(r1 - r2 + r3 + 1, r3, a2, b2));
                }
            }
            s.insert({l1, r1, a1, b1, 1, r1 - l1 + 1});
            fw.update(index(r1), sum(1, r1 - l1 + 1, a1, b1));
        } else {
            auto [l2, r2, a2, b2, l3, r3] = *s.lower_bound({0, r1, 0, 0, 0, 0});

            if (l1 < l2) {
                auto rq = fw.pref_sum(index(r1)) - fw.pref_sum(index(l1 - 1));

                if (r1 != r2) rq += sum(l3, r1 - l2 + l3, a2, b2);
                auto [l2, r2, a2, b2, l3, r3] = *s.lower_bound({0, l1, 0, 0, 0, 0});
                if (l1 != l2) rq -= sum(l3, l1 - l2 + l3 - 1, a2, b2);

                cout << rq << "\n";
            } else cout << sum(l1 - l2 + l3, r1 - r2 + r3, a2, b2) << "\n";
        }
}
