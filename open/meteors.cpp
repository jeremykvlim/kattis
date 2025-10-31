#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<long long, long long>> meteors(n);
    for (auto &[t, x] : meteors) cin >> t >> x;
    sort(meteors.begin(), meteors.end());

    vector<long long> t_20;
    vector<vector<long long>> x_20;
    for (int i = 0; i < n;) {
        auto t = meteors[i].first;
        vector<long long> x;
        for (; i < n && meteors[i].first == t; i++) x.emplace_back(meteors[i].second * 20);
        sort(x.begin(), x.end());
        t_20.emplace_back(t * 20);
        x_20.emplace_back(x);
    }

    auto feasible = [&](auto d_20) {
        set<array<long long, 3>> s;
        priority_queue<array<long long, 3>, vector<array<long long, 3>>, greater<>> pq;
        vector<set<array<long long, 3>>::iterator> its;

        int count = 0;
        auto time = 0LL;
        auto add = [&](auto it_l, auto it_r) {
            auto [xl, tl, l] = *it_l;
            auto [xr, tr, r] = *it_r;
            pq.push({((xr + tr) - (xl - tl)) / 2, l, r});
        };

        auto left = [&](const array<long long, 3> &a, long long t) {
            return a[0] - (t - a[1]);
        };

        auto right = [&](const array<long long, 3> &a, long long t) {
            return a[0] + (t - a[1]);
        };

        its.emplace_back(s.insert({0, 0, count++}).first);
        for (int i = 0; i < t_20.size(); i++) {
            auto T = t_20[i];
            while (!pq.empty() && pq.top()[0] <= T) {
                auto [t, pos_l, pos_r] = pq.top();
                pq.pop();

                auto it_l = its[pos_l], it_r = its[pos_r];
                if (it_l == s.end() || it_r == s.end()) continue;

                auto temp = next(it_l);
                if (temp == s.end() || (*temp)[2] != (*it_r)[2]) continue;

                t = max(t, time);
                auto l = left(*it_l, t), r = right(*it_r, t);
                auto a = (it_l == s.begin() ? s.end() : prev(it_l)), b = next(it_r);
                its[(*it_l)[2]] = its[(*it_r)[2]] = s.end();
                s.erase(it_l);
                s.erase(it_r);

                its.emplace_back(s.insert(b, {l + (r - l) / 2, t - (r - l) / 2, count++}));
                if (a != s.end()) add(a, its.back());
                if (b != s.end()) add(its.back(), b);
                time = t;
            }

            auto xl0 = x_20[i][0] - d_20, xr0 = x_20[i][0] + d_20;
            auto cut = [&]() -> bool {
                if (s.empty()) return false;
                auto it_l = s.lower_bound({xl0, 0, LLONG_MIN});
                if (it_l != s.begin()) {
                    auto temp = prev(it_l);
                    if (xl0 < right(*temp, T)) it_l = temp;
                }

                while (it_l != s.end() && right(*it_l, T) <= xl0) it_l++;
                if (it_l == s.end() || left(*it_l, T) >= xr0) return true;

                auto it_r = it_l;
                while (it_r != s.end() && left(*it_r, T) < xr0) it_r++;

                auto l = left(*it_l, T), r = right(*prev(it_r), T);
                auto a = (it_l == s.begin() ? s.end() : prev(it_l)), b = it_r;

                for (auto k = it_l; k != it_r; k++) its[(*k)[2]] = s.end();
                s.erase(it_l, it_r);

                if (xl0 > l) {
                    its.emplace_back(s.insert(b, {l + (xl0 - l) / 2, T - (xl0 - l) / 2, count++}));
                    auto p = (its.back() == s.begin() ? s.end() : prev(its.back())), q = next(its.back());
                    if (p != s.end()) add(p, its.back());
                    if (q != s.end()) add(its.back(), q);
                }
                if (xr0 < r) {
                    its.emplace_back(s.insert(b, {xr0 + (r - xr0) / 2, T - (r - xr0) / 2, count++}));
                    auto p = (its.back() == s.begin() ? s.end() : prev(its.back())), q = next(its.back());
                    if (p != s.end()) add(p, its.back());
                    if (q != s.end()) add(its.back(), q);
                }
                if (xl0 < l && xr0 > r && a != s.end() && b != s.end()) add(a, b);
                return !s.empty();
            };

            for (int j = 1; j < x_20[i].size(); j++) {
                auto xl1 = x_20[i][j] - d_20, xr1 = x_20[i][j] + d_20;
                if (xl1 <= xr0) xr0 = max(xr0, xr1);
                else {
                    if (!cut()) return false;
                    tie(xl0, xr0) = tie(xl1, xr1);
                }
            }
            if (!cut()) return false;
            time = T;
        }
        return !s.empty();
    };

    long long l = 0, r = 0, m;
    for (auto [t, x] : meteors) r = max(r, t + abs(x));
    r *= 10;
    while (l + 1 < r) {
        m = l + (r - l) / 2;

        if (feasible(2 * m)) l = m;
        else r = m;
    }
    cout << fixed << setprecision(1) << (feasible(2 * l + 1) ? r : l) / 10.;
}
