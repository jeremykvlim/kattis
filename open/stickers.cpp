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

    string message;
    int n;
    cin >> message >> n;

    vector<string> word(n);
    vector<int> price(n);
    for (int i = 0; i < n; i++) cin >> word[i] >> price[i];

    int m = message.size();
    vector<vector<pair<pair<int, int>, int>>> all(m), cover(m);
    for (int i = 0; i < m; i++) {
        unordered_map<pair<int, int>, int, Hash> cheapest;
        for (int j = 0; j < n; j++) {
            int len = word[j].size();
            if (i + len <= m) {
                int mask = 0;
                for (int k = 0; k < len; k++)
                    if (word[j][k] == message[i + k]) mask |= 1 << k;

                auto it = cheapest.find({len, mask});
                if (it == cheapest.end() || it->second > price[j]) cheapest[{len, mask}] = price[j];
            }
        }

        for (auto [s, p] : cheapest) {
            all[i].emplace_back(s, p);
            if (s.second & 1) cover[i].emplace_back(s, p);
        }
    }

    auto overlap = [&](auto s1, auto s2) {
        auto [len1, mask1] = s1;
        auto [len2, mask2] = s2;
        int len = min(len1, len2), mask = (1 << len) - 1, o1 = mask1 & mask, o2 = mask2 & mask;
        if ((o1 | o2) != mask) return false;
        return ((o1 | o2) == o1) || ((o1 | o2) == o2);
    };

    unordered_map<pair<pair<int, int>, pair<int, int>>, long long, Hash> dp, temp;
    dp[{{0, 0}, {0, 0}}] = 0;
    for (int i = 0; i < m; i++) {
        temp.clear();
        for (auto &[stickers, p] : dp) {
            auto [s1, s2] = stickers;
            auto [len1, mask1] = s1;
            auto [len2, mask2] = s2;

            auto shift = [&](auto s) -> pair<int, int> {
                auto [len, mask] = s;
                if (len <= 1) return {0, 0};
                return {len - 1, mask >> 1};
            };

            auto update = [&](const auto &key, const long long &val) {
                auto it = temp.find(key);
                if (it == temp.end() || it->second > val) temp[key] = val;
            };
            if ((mask1 & 1) || (mask2 & 1)) update(minmax(shift(s1), shift(s2)), p);

            if (!len1 || !len2) {
                if (!len1) swap(s1, s2);

                for (auto [s3, p3] : ((mask1 & 1) || (mask2 & 1) ? all[i] : cover[i]))
                    if (overlap(s1, s3)) update(minmax(shift(s1), shift(s3)), p + p3);

                if (!len1 && !len2)
                    for (auto &[s3, p3] : all[i])
                        for (auto &[s4, p4] : cover[i])
                            if (overlap(s3, s4)) update(minmax(shift(s3), shift(s4)), p + p3 + p4);
            }
        }

        dp = temp;
        if (dp.empty()) break;
    }

    auto it = dp.find({{0, 0}, {0, 0}});
    if (it == dp.end()) cout << "IMPOSSIBLE";
    else cout << it->second;
}
