#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int sum = 0;
    vector<pair<int, char>> fences(n);
    for (auto &[l, color] : fences) {
        cin >> l >> color;

        sum += l;
    }

    vector<array<int, 3>> states;
    vector<vector<int>> states_by_sum(sum + 1);
    vector<vector<vector<int>>> indices(sum + 1, vector<vector<int>>(sum + 1, vector<int>(sum + 1, -1)));
    for (int a = 0; a <= sum; a++)
        for (int b = 0; a + b <= sum; b++)
            for (int c = 0; a + b + c <= sum; c++) {
                states.push_back({a, b, c});
                states_by_sum[a + b + c].emplace_back(indices[a][b][c] = states.size() - 1);
            }

    int k = states.size();
    vector<vector<vector<long long>>> dp(k);
    for (int i = 0; i < k; i++) {
        auto [a, b, c] = states[i];
        int a_cap = min(a, n), b_cap = min(b, n);
        dp[i].resize(2 * a_cap + 1, vector<long long>(2 * b_cap + 1, 0));
    }

    auto mask = (1LL << (2 * n + 1)) - 1;
    vector<long long> reachable(k, 0);
    dp[0][0][0] = reachable[0] = 1 << n;
    for (auto [l, color] : fences) {
        int d = color == 'r' ? 1 : -1;
        for (int s = sum - l; ~s; s--) {
            for (int i : states_by_sum[s]) {
                if (!reachable[i]) continue;

                auto [a, b, c] = states[i];
                int a_cap = min(a, n), b_cap = min(b, n);
                if (a + l <= sum) {
                    int j = indices[a + l][b][c], a_cap2 = min(a + l, n), shift = a_cap2 - a_cap + d, a1 = max(0, -shift), a2 = min(2 * a_cap, 2 * a_cap2 - shift);
                    if (a1 <= a2) {
                        for (int ia = a1; ia <= a2; ia++)
                            for (int ib = 0; ib <= 2 * b_cap; ib++) dp[j][ia + shift][ib] |= dp[i][ia][ib];
                        reachable[j] |= reachable[i];
                    }
                }

                if (b + l <= sum) {
                    int j = indices[a][b + l][c], b_cap2 = min(b + l, n), shift = b_cap2 - b_cap + d, b1 = max(0, -shift), b2 = min(2 * b_cap, 2 * b_cap2 - shift);
                    if (b1 <= b2) {
                        for (int ia = 0; ia <= 2 * a_cap; ia++)
                            for (int ib = b1; ib <= b2; ib++) dp[j][ia][ib + shift] |= dp[i][ia][ib];
                        reachable[j] |= reachable[i];
                    }
                }

                if (c + l <= sum) {
                    int j = indices[a][b][c + l];

                    if (d == 1) {
                        for (int ia = 0; ia <= 2 * a_cap; ia++)
                            for (int ib = 0; ib <= 2 * b_cap; ib++) dp[j][ia][ib] |= (dp[i][ia][ib] << 1) & mask;
                        reachable[j] |= (reachable[i] << 1) & mask;
                    } else {
                        for (int ia = 0; ia <= 2 * a_cap; ia++)
                            for (int ib = 0; ib <= 2 * b_cap; ib++) dp[j][ia][ib] |= (dp[i][ia][ib] >> 1) & mask;
                        reachable[j] |= (reachable[i] >> 1) & mask;
                    }
                }
            }
        }
    }

    int product = 0;
    vector<array<int, 3>> deltas{{0, 0, 0}, {0, 1, -1}, {0, -1, 1}, {1, -1, 0}, {-1, 1, 0}, {1, 0, -1}, {-1, 0, 1}};
    for (int i = 0; i < k; i++) {
        if (!reachable[i]) continue;

        auto [a, b, c] = states[i];
        if (!a || !b || !c || a + b + c <= 2 * max({a, b, c})) continue;

        int a_cap = min(a, n), b_cap = min(b, n);
        for (auto [da, db, dc] : deltas) {
            int ia = a_cap + da, ib = b_cap + db, m = n + dc;
            if (0 <= ia && ia <= 2 * a_cap && 0 <= ib && ib <= 2 * b_cap && 0 <= m && m <= 2 * n)
                if ((dp[i][ia][ib] >> m) & 1) {
                    product = max(product, a * b * c);
                    break;
                }
        }
    }
    cout << product;
}
