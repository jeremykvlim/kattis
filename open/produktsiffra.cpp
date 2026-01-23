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

    long long l, r;
    cin >> l >> r;

    vector<long long> p2(46, 1), p3(46, 1), p5(46, 1), p7(46, 1);
    for (int i = 1; i <= 45; i++) {
        p2[i] = p2[i - 1] * 2;
        p3[i] = p3[i - 1] * 3;
        p5[i] = p5[i - 1] * 5;
        p7[i] = p7[i - 1] * 7;
    }

    vector<int> pow2{0, 0, 1, 0, 2, 0, 1, 0, 3, 0}, pow3{0, 0, 0, 1, 0, 0, 1, 0, 0, 2},
                pow5{0, 0, 0, 0, 0, 1, 0, 0, 0, 0}, pow7{0, 0, 0, 0, 0, 0, 0, 1, 0, 0};

    unordered_map<array<int, 4>, int, Hash> memo;
    auto counts = [&](long long x) {
        array<unordered_map<array<int, 4>, long long, Hash>, 2> dp;
        dp[1][{0, 0, 0, 0}] = 1;
        for (char c : to_string(x)) {
            array<unordered_map<array<int, 4>, long long, Hash>, 2> temp;
            int digit = c - '0';
            for (int tight = 0; tight <= 1; tight++) {
                int limit = tight ? digit : 9;
                for (auto [state, count] : dp[tight]) {
                    auto [_2, _3, _5, _7] = state;
                    for (int d = 0; d <= limit; d++) temp[tight && d == limit][{_2 + pow2[d], _3 + pow3[d], _5 + pow5[d], _7 + pow7[d]}] += count;
                }
            }
            dp = temp;
        }

        array<long long, 10> a{};
        for (int tight = 0; tight <= 1; tight++) {
            for (auto &[state, count] : dp[tight]) {
                if (memo[state]) {
                    a[memo[state]] += count;
                    continue;
                }
                auto d = p2[state[0]] * p3[state[1]] * p5[state[2]] * p7[state[3]];
                while (d > 9) {
                    auto product = 1LL;
                    for (; d; d /= 10) {
                        int digit = d % 10;
                        if (digit) product *= digit;
                    }
                    d = product;
                }
                a[memo[state] = d] += count;
            }
        }
        return a;
    };
    auto a_r = counts(r), a_l = counts(l - 1);
    for (int i = 1; i <= 9; i++) cout << a_r[i] - a_l[i] << " ";
}
