#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int l, r;
    cin >> l >> r;

    auto key = [&](int bit, bool a_tight, bool b_tight) {
        return bit * 4 + a_tight * 2 + b_tight;
    };

    auto dp = [&](int a_ub, int b_ub) {
        if (a_ub < 0 || b_ub < 0) return 0LL;

        int msb = bit_width((unsigned) max(a_ub, b_ub)) - 1;
        unordered_map<int, array<long long, 4>> memo;
        auto dfs = [&](auto &&self, int bit, bool a_tight = true, bool b_tight = true) -> array<long long, 4> {
            if (bit < 0) return {1, 0, 0, 0};
            if (memo.count(key(bit, a_tight, b_tight))) return memo[key(bit, a_tight, b_tight)];

            array<long long, 4> state{0, 0, 0, 0};
            int max_bit_a = a_tight ? (a_ub >> bit) & 1 : 1, max_bit_b = b_tight ? (b_ub >> bit) & 1 : 1;
            for (int bit_a = 0; bit_a <= max_bit_a; bit_a++)
                for (int bit_b = 0; bit_b <= max_bit_b; bit_b++) {
                    auto s = self(self, bit - 1, a_tight && (bit_a == max_bit_a), b_tight && (bit_b == max_bit_b));

                    for (int k = 0; k < 4; k++) {
                        if (!s[k]) continue;

                        int carry_in = (k >> 1) & 1, borrow_in = k & 1;
                        if ((bit_a ^ bit_b) ^ borrow_in && !((bit_a ^ bit_b) ^ carry_in)) continue;

                        int carry_out = (bit_a & bit_b) | ((bit_a ^ bit_b) & carry_in), borrow_out = (~bit_a & (bit_b | borrow_in)) | (bit_b & borrow_in);
                        state[(carry_out << 1) | borrow_out] += s[k];
                    }
                }
            return memo[key(bit, a_tight, b_tight)] = state;
        };

        auto count = dfs(dfs, msb);
        return count[0] + count[2];
    };
    cout << dp(r, r) - dp(l - 1, r) - dp(r, l - 1) + dp(l - 1, l - 1);
}
