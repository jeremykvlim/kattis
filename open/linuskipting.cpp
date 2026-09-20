#include <bits/stdc++.h>
using namespace std;

template <typename T>
struct LARSCH {
    int n, lg, i;
    vector<pair<T, int>> dp, cols;
    vector<int> offset, size, pos;

    LARSCH(int n) : n(n), lg(__lg(n)), i(0), dp(lg), offset(lg + 1), size(lg), pos(lg) {
        for (int b = 0; b < lg; b++) offset[b + 1] = offset[b] + (n >> (b + 1));
        cols.resize(offset[lg]);
    }

    pair<T, int> query(auto &&get, auto &&cmp) {
        int b = 0;
        pair<T, int> optimum;
        for (;;) {
            int r = i >> b, limit = (r + 1) >> 1, col = !b ? i : cols[offset[b - 1] + r].second;
            if (b == lg) {
                int row = ((r + 1) << b) - 1;
                if (!b) optimum = {get(row, col), col};
                else optimum = cols[offset[b - 1] + r];
                b--;
                break;
            }

            if (r & 1) {
                int row = (limit << (b + 1)) - 1;
                pair<T, int> p;
                if (!b) p = {get(row, col), col};
                else p = cols[offset[b - 1] + r];
                if (pos[b] == r || cmp(dp[b], p)) {
                    pos[b] = r;
                    dp[b] = p;
                }
            }

            if (pos[b] == r) {
                size[b] = limit;
                if (b + 1 < lg) pos[b + 1] = limit;
            }

            int temp = size[b];
            for (; size[b] > limit; size[b]--) {
                int row = (size[b] << (b + 1)) - 1;
                pair<T, int> p{get(row, col), col};
                if (!cmp(cols[offset[b] + size[b] - 1], p)) break;
                cols[offset[b] + size[b] - 1] = p;
            }

            if (temp != size[b]) size[b]++;
            else {
                int row = ((size[b] + 1) << (b + 1)) - 1;
                if (row < n) cols[offset[b] + size[b]++] = {get(row, col), col};
            }

            if (r & 1) {
                optimum = dp[b];
                b--;
                break;
            }

            if ((limit + 1) << (b + 1) > n) {
                optimum.second = col;
                break;
            }

            b++;
        }

        for (; ~b; b--) {
            dp[b] = optimum;
            int r = i >> b, row = ((r + 1) << b) - 1, stop = dp[b].second, col = !b ? pos[b] : cols[offset[b - 1] + pos[b]].second;
            auto next = [&]() -> pair<T, int> {
                if (!b || pos[b] != r) return {get(row, col), col};
                return cols[offset[b - 1] + r];
            };
            optimum = next();
            if (col == stop) continue;
            for (pos[b]++;; pos[b]++) {
                col = !b ? pos[b] : cols[offset[b - 1] + pos[b]].second;
                auto p = next();
                if (cmp(optimum, p)) optimum = p;
                if (col == stop) break;
            }
        }
        i++;
        return optimum;
    }
};

ostream & operator<<(ostream &stream, const __int128 &v) {
    if (!v) return stream << 0;
    if (v == numeric_limits<__int128>::min()) return stream << "-170141183460469231731687303715884105728";

    auto temp = v;
    if (v < 0) {
        stream << '-';
        temp = -temp;
    }

    string s;
    while (temp) {
        s += '0' + (temp % 10);
        temp /= 10;
    }
    reverse(s.begin(), s.end());
    return stream << s;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, b, s;
    cin >> n >> b >> s;

    vector<long long> pref(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        int l;
        cin >> l;

        pref[i] = pref[i - 1] + l + s;
    }

    vector<__int128> dp(n + 1);
    auto get = [&](int row, int col) -> __int128 {
        auto x = pref[row + 1] - pref[col] - s;
        if (x > b) return ((__int128) 1) << 100;
        return dp[col] + (__int128) (b - x) * (b - x);
    };

    auto cmp = [&](const auto &p1, const auto &p2) {
        return p1.first != p2.first ? p1.first > p2.first : p1.second < p2.second;
    };
    LARSCH<__int128> larsch(n);
    for (int i = 1; i <= n; i++) dp[i] = larsch.query(get, cmp).first;
    cout << dp[n];
}