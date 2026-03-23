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

    string line;
    getline(cin, line);

    vector<string> cards;
    stringstream ss(line);
    while (getline(ss, line, ',')) cards.emplace_back(line);

    unordered_map<char, int> values{{'A', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9}, {'0', 10}, {'J', 11}, {'Q', 12}, {'K', 13}},
                             suits{{'H', 0}, {'S', 1}, {'T', 2}, {'L', 3}};

    auto points = [&](int value) {
        if (value == 1) return 20;
        if (value == 2) return 40;
        if (value > 10) return 10;
        return 5;
    };
    int n = cards.size();
    vector<int> suit(n), value(n);
    for (int i = 0; i < n; i++) {
        suit[i] = suits[cards[i][0]];
        value[i] = values[cards[i][1]];
    }

    vector<int> score(1 << n, 0);
    for (auto mask = 1U; mask < 1 << n; mask++) {
        int i = countr_zero(mask);
        score[mask] = score[mask ^ (1 << i)] + points(value[i]);
    }

    vector<vector<long long>> wild_masks(1 << n);
    for (auto mask = 1U; mask < 1 << n; mask++) {
        int m = popcount(mask);
        if (m <= 2) continue;

        auto card = [&](int s, int v) {
            return 1LL << (s * 13 + (v - 1));
        };

        int wild = 0;
        auto fixed_mask = 0LL;
        vector<pair<int, int>> fixed;
        for (int i = 0; i < n; i++)
            if ((mask >> i) & 1) {
                if (value[i] == 2) wild++;
                else {
                    fixed_mask |= card(suit[i], value[i]);
                    fixed.emplace_back(suit[i], value[i]);
                }
            }

        if (m <= 4) {
            int suit_mask = 0, same_value = 0;
            for (auto [s, v] : fixed) {
                if ((suit_mask >> s) & 1 || (same_value && same_value != v)) {
                    same_value = -1;
                    break;
                }
                suit_mask |= 1 << s;
                same_value = v;
            }

            if (~same_value) {
                vector<int> remaining;
                for (int s = 0; s < 4; s++)
                    if (!((suit_mask >> s) & 1)) remaining.emplace_back(s);

                for (int v = 1; v <= (same_value ? 1 : 13); v++)
                    for (auto use = 0U; use < 1 << remaining.size(); use++)
                        if (popcount(use) == wild) {
                            auto w = 0LL;
                            for (int i = 0; i < remaining.size(); i++)
                                if ((use >> i) & 1) w |= card(remaining[i], v);
                            wild_masks[mask].emplace_back(w);
                        }
            }
        }

        int value_mask = 0, same_suit = -1;
        for (auto [s, v] : fixed) {
            if ((value_mask >> v) & 1 || (~same_suit && same_suit != s)) {
                same_suit = -2;
                break;
            }
            value_mask |= 1 << v;
            same_suit = s;
        }

        if (same_suit != -2)
            for (int s = 0; s < 4; s++) {
                if (~same_suit && same_suit != s) continue;
                for (int val = 1; val <= 13; val++) {
                    int seen = 0;
                    auto w = 0LL;
                    for (int i = 0; i < m; i++) {
                        int v = (val + i) % 13;
                        seen |= 1 << v;
                        w |= card(s, v);
                    }

                    if (all_of(fixed.begin(), fixed.end(), [&](auto p) { return ((seen >> p.second) & 1) && p.first == s; })) wild_masks[mask].emplace_back(w ^ fixed_mask);
                }
            }

        sort(wild_masks[mask].begin(), wild_masks[mask].end());
        wild_masks[mask].erase(unique(wild_masks[mask].begin(), wild_masks[mask].end()), wild_masks[mask].end());
    }

    int p = 0;
    for (auto mask = 1U; mask + 1 < 1 << n; mask++) {
        unordered_map<pair<int, long long>, bool, Hash> memo;
        auto dfs = [&](auto &&self, unsigned m1, long long used = 0) -> bool {
            if (!m1) return true;

            auto [it, inserted] = memo.try_emplace({m1, used});
            if (!inserted) return it->second;

            for (int m2 = m1; m2; --m2 &= m1)
                if ((m2 >> countr_zero(m1)) & 1)
                    for (auto m3 : wild_masks[m2])
                        if (!(m3 & used))
                            if (self(self, m1 ^ m2, m3 | used)) return it->second = true;

            return it->second = false;
        };
        if (dfs(dfs, mask)) {
            if (popcount(mask) == n - 1) score[mask] += 50;
            p = max(p, score[mask]);
        }
    }
    cout << p;
}
