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

    string d0, e0;
    getline(cin, d0);
    getline(cin, e0);

    int n;
    cin >> n;

    vector<string> ciphers(n), ciphers_inv(n, string(26, ' '));
    for (int i = 0; i < n; i++) {
        cin >> ciphers[i];

        for (int c = 0; c < 26; c++) ciphers_inv[i][ciphers[i][c] - 'a'] = c + 'a';
    }

    cin.ignore();
    string e1;
    getline(cin, e1);

    string d, e;
    vector<bool> seen(26, false);
    for (int i = 0; i < d0.size(); i++)
        if (isalpha(d0[i]) && !seen[d0[i] - 'a']) {
            seen[d0[i] - 'a'] = true;
            d += d0[i];
            e += e0[i];
        }

    unordered_map<pair<string, int>, string, Hash> memo;
    auto dfs1 = [&](auto &&self, string d, int mask = 0, string order_rev = "") -> void {
        if (order_rev.size() == n / 2) {
            memo[{d, mask}] = order_rev;
            return;
        }

        for (int i = 0; i < n; i++)
            if (!((mask >> i) & 1)) {
                auto temp = d;
                for (char &c : temp) c = ciphers[i][c - 'a'];
                self(self, temp, mask | (1 << i), (char) i + order_rev);
            }
    };
    dfs1(dfs1, d);

    string order;
    auto dfs2 = [&](auto &&self, string e, int mask = 0, string order_rev = "") -> bool {
        if (order_rev.size() == n - n / 2) {
            auto it = memo.find({e, ((1 << n) - 1) ^ mask});
            if (it == memo.end()) return false;
            order = order_rev + it->second;
            return true;
        }

        for (int i = 0; i < n; i++)
            if (!((mask >> i) & 1)) {
                auto temp = e;
                for (char &c : temp) c = ciphers_inv[i][c - 'a'];
                if (self(self, temp, mask | (1 << i), order_rev + (char) i)) return true;
            }
        return false;
    };
    dfs2(dfs2, e);

    auto d1 = e1;
    for (int i : order)
        for (char &c : d1)
            if (isalpha(c)) c = ciphers_inv[i][c - 'a'];
    cout << d1;
}
