#include <bits/stdc++.h>
using namespace std;

long long floor_sum(long long n, int a, int c, int d) {
    auto sum = 0LL;
    while (n) {
        if (a >= d) {
            sum += (a / d) * (__int128) (n * (n - 1)) / 2;
            a %= d;
        }
        if (c >= d) {
            sum += n * (c / d);
            c %= d;
        }

        auto y_max = a * n + c;
        if (y_max < d) break;
        n = y_max / d;
        c = y_max % d;
        swap(a, d);
    }
    return sum;
}

__int128 linear_mod_sum(int a, int n, int mod) {
    return (__int128) a * n * (n - 1) / 2 - (__int128) mod * floor_sum(n, a, 0, mod);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;

    map<int, array<int, 4>> intervals;
    while (q--) {
        int c, l, r;
        cin >> c >> l >> r;

        auto split = [&](int i) -> void {
            if (i > n) return;

            auto it = intervals.upper_bound(i);
            if (it == intervals.begin()) return;

            auto [L, interval] = *(--it);
            if (L == i || i > interval[0]) return;

            it->second[0] = i - 1;
            intervals[i] = interval;
        };
        split(l);
        split(r + 1);

        if (c == 1) {
            int a, b;
            cin >> a >> b;

            intervals.erase(intervals.lower_bound(l), intervals.lower_bound(r + 1));
            intervals[l] = {r, l - 1, a, b};
        } else {
            auto stones = 0LL;
            for (auto it = intervals.lower_bound(l); it != intervals.end() && it->first <= r; it++) {
                auto [L, interval] = *it;
                auto [R, l_original, a, b] = interval;
                stones += linear_mod_sum(a, min(R, r) - l_original + 1, b) - linear_mod_sum(a, max(L, l) - l_original, b);
            }
            cout << stones << "\n";
        }
    }
}
