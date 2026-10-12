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

template <typename T>
vector<int> prefix_function(const T &s) {
    int n = s.size();
    vector<int> pi(n);
    for (int i = 1; i < n; i++) {
        int j = pi[i - 1];
        while (j && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }

    return pi;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t, n;
    cin >> t >> n;

    vector<vector<long long>> days(t, vector<long long>(5, 1e18));
    vector<long long> initial(5, 1e18);
    while (n--) {
        int x, p;
        long long l;
        cin >> x >> l >> p;

        initial[p - 1] = min(initial[p - 1], x + l);
        days[x % t][p - 1] = min(days[x % t][p - 1], x % t + l);
    }

    int q;
    cin >> q;

    for (int p = 0; p < 5; p++) {
        long long d = 1e18;
        for (int x = 0; x < t; x++) d = min(d, days[x][p]);

        if (d != 1e18) d += t;
        for (int x = t - 1; ~x; x--) {
            d = min(d, days[x][p]);
            days[x][p] = d == 1e18 ? 1e18 : d - x;
        }
    }

    auto pi = prefix_function(days);
    int period = t - pi.back();
    if (t % period) period = t;

    vector<long long> dp{0};
    unordered_map<array<long long, 5>, int, Hash> seen;
    int l = -1, r = -1;
    for (int k = 1; dp.back() <= 1e11; k++) {
        long long y = 1e18;
        for (int p = 1; p <= 5; p++) {
            auto x = dp[max(0, k - p)];
            y = min(y, x ? x + days[x % t][p - 1] : initial[p - 1]);
        }
        dp.emplace_back(y);

        if (k >= 5) {
            array<long long, 5> state{y % period};
            for (int p = 1; p < 5; p++) state[p] = y - dp[k - p];

            auto [it, inserted] = seen.emplace(state, k);
            if (!inserted) {
                l = it->second;
                r = k;
                break;
            }
        }
    }

    while (q--) {
        long long z;
        cin >> z;

        if (!~l || z < dp[l]) cout << upper_bound(dp.begin(), dp.end(), z) - dp.begin() - 1 << "\n";
        else {
            auto delta = dp[r] - dp[l], count = (z - dp[l]) / delta;
            cout << (upper_bound(dp.begin() + l, dp.begin() + r, z - count * delta) - dp.begin() - 1) + count * (r - l) << "\n";
        }
    }
}