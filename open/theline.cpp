#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<pair<int, int>> pairs(k);
    vector<int> ls(k), rs(k);
    auto total = 0LL;
    for (int i = 0; i < k; i++) {
        int s, t;
        cin >> s >> t;

        auto &[l, r] = pairs[i];
        tie(l, r) = minmax(s, t);
        ls[i] = l;
        rs[i] = r;
        total += r - l;
    }
    sort(pairs.begin(), pairs.end());
    sort(ls.begin(), ls.end());
    ls.erase(unique(ls.begin(), ls.end()), ls.end());
    sort(rs.begin(), rs.end());
    rs.erase(unique(rs.begin(), rs.end()), rs.end());

    int s = rs.size();
    vector<array<int, 4>> pos(k);
    for (int i = 0; i < k; i++) {
        auto [l, r] = pairs[i];
        pos[i] = {
                      (int) (upper_bound(rs.begin(), rs.end(), r) - rs.begin()),
                      (int) (upper_bound(rs.begin(), rs.end(), 2 * r - ls[0] - 2) - rs.begin()),
                      (int) (upper_bound(rs.begin(), rs.end(), 2 * l - ls[0] + 1) - rs.begin()),
                      (int) (upper_bound(rs.begin(), rs.end(), 2 * (r - l) + ls[0] - 2) - rs.begin())
                 };
    }

    auto saved = 0LL;
    vector<int> slope(s + 1, 0);
    vector<long long> intercept(s + 1, 0);
    int mid = 0, start = 0;
    auto add = [&](int l, int r, int m, long long c) {
        if (l >= r) return;
        slope[l] += m;
        slope[r] -= m;
        intercept[l] += c;
        intercept[r] -= c;
    };

    for (int l1 : ls) {
        for (; mid < k && pairs[mid].first <= l1; mid++);
        for (; start < s && rs[start] <= l1; start++);

        fill(slope.begin(), slope.end(), 0);
        fill(intercept.begin(), intercept.end(), 0);

        for (int i = 0; i < mid; i++) {
            auto [l2, r] = pairs[i];
            if (r <= l1 + 1) continue;

            for (; pos[i][1] && rs[pos[i][1] - 1] > 2 * r - l1 - 2; pos[i][1]--);
            add(start, pos[i][0], 1, -l1 - 1);
            add(pos[i][0], pos[i][1], -1, 2 * r - l1 - 1);
        }

        for (int i = mid; i < k; i++) {
            auto [l2, r] = pairs[i];
            if (r + l1 <= 2 * l2 + 1) continue;

            for (; pos[i][2] && rs[pos[i][2] - 1] > 2 * l2 - l1 + 1; pos[i][2]--);
            for (; pos[i][3] < s && rs[pos[i][3]] < 2 * (r - l2) + l1 - 1; pos[i][3]++);
            add(pos[i][2], pos[i][0], 1, l1 - 2 * l2 - 1);
            add(pos[i][0], pos[i][3], -1, 2 * (r - l2) + l1 - 1);
        }

        long long m = 0, c = 0;
        for (int i = start; i < s; i++) {
            m += slope[i];
            c += intercept[i];
            saved = max(saved, m * rs[i] + c);
        }
    }
    cout << total - saved;
}
