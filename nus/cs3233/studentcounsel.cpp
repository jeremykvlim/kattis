#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<pair<int, int>> T(n), S(m);
    auto read = [&](auto &intervals) {
        for (auto &[Li, Ri] : intervals) {
            cin >> Li >> Ri;

            Li *= 2;
            Ri *= 2;
        }
    };
    read(T);
    read(S);

    int l = 0, r = 0, j = 0;
    auto [LS, RS] = S[j];

    auto check = [&](int L, int R, auto &all, int &total) -> void {
        if (L < R) {
            all.emplace_back(L, R);
            total += R - L;
        }
    };

    int total_t = 0, total_s = 0, total_overlap = 0;
    vector<pair<int, int>> overlap, trixie, silver, t, s;
    for (int i = 0; i < n; i++) {
        auto [LT, RT] = T[i];
        for (; j < m && S[j].second <= LT; j++) {
            tie(LS, RS) = S[j];
            r = max(LS, r);
            check(r, RS, silver, total_s);
        }

        tie(LS, RS) = S[j];
        if (j < m && LS < LT) {
            r = max(r, LS);
            check(r, LT, silver, total_s);
        }

        l = LT;

        auto add_overlap = [&](int end) -> void {
            if (max(LT, LS) < end) {
                overlap.emplace_back(max(LT, LS), end);
                total_overlap += end - max(LT, LS);
            }
        };

        for (; j < m && S[j].second <= RT; j++) {
            tie(LS, RS) = S[j];
            check(l, LS, trixie, total_t);
            l = RS;
            add_overlap(RS);
        }

        tie(LS, RS) = S[j];
        if (j < m && LS < RT) {
            check(l, LS, trixie, total_t);
            add_overlap(RT);
        } else check(l, RT, trixie, total_t);

        r = RT;
    }

    for (; j < m; j++) {
        tie(LS, RS) = S[j];
        r = max(r, LS);
        check(r, RS, silver, total_s);
    }

    int total = min(min(total_t + total_overlap, total_s + total_overlap), (total_t + total_s + total_overlap) / 2);
    total_t = total_s = 0;

    auto valid = [&](int L, int R, int t) {
        return total - t >= R - L;
    };

    auto exceeds = [&](int t) {
        return total > t;
    };

    auto add = [&](int L, int R, auto &intervals, int &t) {
        intervals.emplace_back(L, R);
        t += R - L;
    };

    auto trim = [&](auto all, auto &intervals, int &t) {
        for (auto [L, R] : all) {
            if (valid(L, R, t)) add(L, R, intervals, t);
            else if (exceeds(t)) add(L, L + total - t, intervals, t);
        }
    };
    trim(trixie, t, total_t);
    trim(silver, s, total_s);

    for (auto [L, R] : overlap) {
        if (valid(L, R, total_t)) add(L, R, t, total_t);
        else if (exceeds(total_t)) {
            t.emplace_back(L, L + total - total_t);

            if (exceeds(total_s)) {
                if (valid(L + total - total_t, R, total_s)) add(L + total - total_t, R, s, total_s);
                else add(L + total - total_t, L + total - total_t + total - total_s, s, total_s);
            }

            total_t = total;
        } else if (valid(L, R, total_s)) add(L, R, s, total_s);
        else if (exceeds(total_s)) add(L, L + total - total_s, s, total_s);
    }

    sort(t.begin(), t.end());
    sort(s.begin(), s.end());
    auto merge = [&](auto &intervals) {
        vector<pair<int, int>> temp;
        for (auto [L, R] : intervals) {
            if (temp.empty() || temp.back().second != L) temp.emplace_back(L, R);
            else temp.back().second = R;
        }
        intervals = temp;
    };
    merge(t);
    merge(s);

    cout << t.size() << " " << s.size() << "\n";
    auto write = [&](auto &intervals) {
        for (auto &[li, ri] : intervals) cout << fixed << setprecision(1) << (double) li / 2 << " " << (double) ri / 2 << "\n";
    };
    write(t);
    write(s);
}
