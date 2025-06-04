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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> groups(n);
    unordered_map<vector<int>, int, Hash> memo{{groups, 0}};
    int people = 0;
    for (int i = 0; i < n; i++) {
        cin >> groups[i];

        people += groups[i] * (i + 1);
    }

    vector<int> pref(14);
    iota(pref.begin(), pref.end(), 0);
    for (int i = 1; i < 14; i++) pref[i] = pref[i - 1] + i + 1;
    if (people > pref[13]) {
        cout << "impossible";
        exit(0);
    }

    auto dp = [&](auto &&self) {
        if (memo.count(groups)) return memo[groups];

        memo[groups] = pref[13];
        for (int i = 0; i < groups.size(); i++)
            if (groups[i]) {
                groups[i]--;
                int s = self(self);
                if (s == pref[13]) continue;
                groups[i]++;

                auto it = upper_bound(pref.begin() + i + 1, pref.end(), s);
                memo[groups] = min(memo[groups], (*it - s > i + 1 ? max(s, *prev(it)) : *it) + i + 2);
            }
        return memo[groups];
    };

    int size = upper_bound(pref.begin(), pref.end(), dp(dp) - 1) - pref.begin();
    cout << (size > 12 ? "impossible" : to_string(size));
}
