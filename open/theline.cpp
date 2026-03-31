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
    sort(ls.begin(), ls.end());
    ls.erase(unique(ls.begin(), ls.end()), ls.end());
    sort(rs.begin(), rs.end());
    rs.erase(unique(rs.begin(), rs.end()), rs.end());

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
    vector<int> slope(rs.size() + 1, 0);
    vector<long long> intercept(rs.size() + 1, 0);
    auto add = [&](int l, int r, int d1, long long d2) {
        if (l >= r) return;
        slope[l] += d1;
        slope[r] -= d1;
        intercept[l] += d2;
        intercept[r] -= d2;
    };
    
    for (int l1 : ls) {
        if (l1 != ls[0])
            for (int i = 0; i < k; i++) {
                auto [l2, r] = pairs[i];
                for (; pos[i][1] && rs[pos[i][1] - 1] > 2 * r - l1 - 2; pos[i][1]--);
                for (; pos[i][2] && rs[pos[i][2] - 1] > 2 * l2 - l1 + 1; pos[i][2]--);
                for (; pos[i][3] < rs.size() && rs[pos[i][3]] < 2 * (r - l2) + l1 - 1; pos[i][3]++);
            }

        fill(slope.begin(), slope.end(), 0);
        fill(intercept.begin(), intercept.end(), 0);
        for (int i = 0; i < k; i++) {
            auto [l2, r] = pairs[i];
            if (l1 >= l2) {
                add(0, pos[i][0], 1, 2LL * 0 - l1 - 1);
                add(pos[i][0], pos[i][1], -1, 2LL * r - l1 - 1);
            } else {
                add(pos[i][2], pos[i][0], 1, 2LL * (0 - l2) + l1 - 1);
                add(pos[i][0], pos[i][3], -1, 2LL * (r - l2) + l1 - 1);
            }
        }

        int m = 0;
        auto c = 0LL;
        for (int i = 0; i < rs.size(); i++) {
            m += slope[i];
            c += intercept[i];
            if (l1 < rs[i]) saved = max(saved, (long long) m * rs[i] + c);
        }
    }
    cout << total - saved;
}
