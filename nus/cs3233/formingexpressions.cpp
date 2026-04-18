#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    array<vector<int>, 5> all;
    for (int i = 0; i < 5; i++)
        for (int a = 20 * i + 20; a > 20 * i; a--) all[i].emplace_back(a);

    array<vector<int>, 5> pref;
    array<vector<bitset<155245>>, 5> dp;
    for (int i = 0; i < 5; i++) {
        int n = all[i].size();
        pref[i].assign(n + 1, 0);
        for (int j = 0; j < n; j++) pref[i][j + 1] = pref[i][j] + all[i][j];

        dp[i].assign(n + 1, {});
        dp[i][n][0] = true;
        for (int l = n - 1; ~l; l--) {
            dp[i][l] |= dp[i][l + 1] << all[i][l];
            for (int m = l; m < n - 1; m++)
                for (int r = m + 1; r < n; r++) dp[i][l] |= dp[i][r + 1] << ((pref[i][m + 1] - pref[i][l]) * (pref[i][r + 1] - pref[i][m + 1]));
        }
    }

    int q;
    cin >> q;

    array<unordered_map<int, string>, 5> memo;
    array<int, 5> base{248, 3926, 12525, 27919, 54132};
    array<int, 4> divs{107, 106 * 105, 104 * 103, 102 * 101};
    array<string, 4> divs_str{"107", "106*105", "104*103", "102*101"};
    while (q--) {
        unsigned long long n;
        cin >> n;

        if (n == 2026) {
            cout << "10+9*8*(7+6+5+4+3+2+1)\n";
            continue;
        }

        auto m = n - 7114540689513829440;
        auto temp = base;
        for (int i = 0; i < 4; i++) {
            temp[i] += m % divs[i];
            m /= divs[i];
        }
        temp[4] += m;

        if (temp[4] > 155244) {
            cout << "-1\n";
            continue;
        }

        array<string, 5> expr;
        for (int i = 0; i < 5; i++) {
            auto [it, inserted] = memo[i].try_emplace(temp[i]);
            if (inserted) {
                auto dfs = [&](auto &&self, int l, int target) -> string {
                    int n = all[i].size();
                    if (l == n) return "";

                    if (target >= all[i][l] && dp[i][l + 1][target - all[i][l]]) {
                        auto s = to_string(all[i][l]), extra = self(self, l + 1, target - all[i][l]);
                        return extra.empty() ? s : s + "+" + extra;
                    }

                    auto concat = [&](int l, int r) {
                        auto s = to_string(all[i][l]);
                        for (int j = l + 1; j <= r; j++) s += "+" + to_string(all[i][j]);
                        return s;
                    };

                    for (int m = l; m < n - 1; m++)
                        for (int r = m + 1; r < n; r++) {
                            int t = target - (pref[i][m + 1] - pref[i][l]) * (pref[i][r + 1] - pref[i][m + 1]);
                            if (t >= 0 && dp[i][r + 1][t]) {
                                auto s = "(" + concat(l, m) + ")*(" + concat(m + 1, r) + ")", extra = self(self, r + 1, t);
                                return extra.empty() ? s : s + "+" + extra;
                            }
                        }

                    return "";
                };
                it->second = dfs(dfs, 0, temp[i]);
            }
            expr[i] = it->second;
        }

        auto wrap = [&](const auto &d, const auto &e) {
            return "(" + d + "*(" + e + "))";
        };
        auto s = wrap(divs_str[3], expr[4]);
        for (int i = 3; i; i--) s = wrap(divs_str[i - 1], "(" + s + "+" + expr[i] + ")");
        cout << "(" << s << "+" << expr[0] << ")\n";
    }
}
