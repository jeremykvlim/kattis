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

    int n;
    cin >> n;

    vector<int> scores(n);
    for (int &s : scores) cin >> s;
    sort(scores.begin(), scores.end());

    unordered_map<vector<int>, __int128, Hash> memo;
    auto dp = [&](auto &&self1, vector<int> seq) -> __int128 {
        auto it = memo.find(seq);
        if (it != memo.end()) return it->second;

        int m = seq.size(), sum = 0;
        for (int i = 0; i < m; i++) {
            int s = seq[i];
            if (s > m - 1) return memo[seq] = 0;
            sum += s;
            if (sum < i * (i + 1) / 2) return memo[seq] = 0;
        }
        if (sum != m * (m - 1) / 2) return memo[seq] = 0;
        if (m <= 1) return memo[seq] = 1;

        int s = seq.back();
        seq.pop_back();
        __int128 ways = 0;
        auto dfs = [&](auto &&self2, int remaining, int i = 0) -> void {
            if (!remaining) {
                auto temp = seq;
                sort(temp.begin(), temp.end());
                ways += self1(self1, temp);
                return;
            }
            if (i == m - 1 || i + remaining > m - 1) return;
            if (seq[i]) {
                seq[i]--;
                self2(self2, remaining - 1, i + 1);
                seq[i]++;
            }
            self2(self2, remaining, i + 1);
        };
        dfs(dfs, m - 1 - s);
        seq.emplace_back(s);
        return memo[seq] = ways;
    };
    cout << dp(dp, scores);
}
