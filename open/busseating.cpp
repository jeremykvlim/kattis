#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, m;
    long long C;
    cin >> n >> k >> m >> C;

    set<pair<int, int>> seats;
    for (int i = 1; i <= n; i++) seats.emplace(0, i);

    int lg = __lg(C);
    while (m--) {
        int a;
        cin >> a;

        auto c = 0LL;
        auto it1 = seats.end();
        for (int o = seats.begin()->first, p = o, q = o; p < min(k, o + lg + 1); p++) {
            auto u = c << (p - o);
            if (u > (C << (q - o))) break;

            auto it2 = seats.lower_bound({p, a});
            if (it2 != seats.begin()) {
                auto it3 = prev(it2);
                if (it3->first == p && (it2 == seats.end() || it2->first != p || 2 * a <= it2->second + it3->second)) it2 = it3;
            }
            if (it2 == seats.end() || it2->first != p) continue;

            int b = it2->second;
            auto utility = (C - abs(a - b)) << (q - o);
            if (u < utility || (u == utility && b < it1->second)) {
                c = C - abs(a - b);
                q = p;
                it1 = it2;
            }
        }

        auto [p, b] = *it1;
        seats.erase(it1);
        if (p + 1 < k) seats.emplace(p + 1, b);
        cout << b << " ";
    }
}
