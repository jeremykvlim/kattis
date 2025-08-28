#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<vector<int>> masks(25);
    auto index = [&](int l, int target) { return l * 5 + target; };
    for (int l = 1; l <= 4; l++)
        for (auto m = 0U; m < 1 << l; m++) masks[index(l, popcount(m))].emplace_back(m);

    int t;
    cin >> t;

    stack<int> st;
    vector<long long> visited(((1 << 26) + 63) >> 6, 0);
    while (t--) {
        unsigned long long n;
        int k;
        cin >> n >> k;

        if (n <= 25) {
            vector<int> dp(n + 1, 1e9), values(n + 1, -1);
            dp[0] = 0;
            for (int i = 0; i <= n; i++)
                if (dp[i] != 1e9)
                    for (int v : {2, 9, 10, 12})
                        if (i + v <= n && dp[i + v] > dp[i] + 1) {
                            dp[i + v] = dp[i] + 1;
                            values[i + v] = v;
                        }

            if (dp[n] == 1e9 || dp[n] > k) {
                cout << "Impossible\n";
                continue;
            }

            while (n) {
                cout << values[n] << " ";
                n -= values[n];
                k--;
            }

            while (k--) cout << "0 ";
            cout << "\n";
            continue;
        }

        int w = bit_width(n);
        if (!(w & 1) && 2 * popcount(n) == w) {
            cout << n << " ";
            while (k-- > 1) cout << "0 ";
            cout << "\n";
            continue;
        }

        int l = min(k, 3);
        vector<long long> values(l, 0);
        auto dfs = [&](auto &&self, int i, int carry = 0, const array<int, 3> &a = {-1, -1, -1}) -> bool {
            for (int j = 0; j < l; j++)
                if (a[j] > i + 1) return false;
            if (!~i) return !carry;

            array<int, 3> temp{a[0] & 63, a[1] & 63, (l == 2 ? 63 : (a[2] & 63))};
            sort(temp.begin(), temp.end());

            int state = (carry << 24) | ((i & 63) << 18) | (temp[0] << 12) | (temp[1] << 6) | temp[2];
            auto [q, r] = make_pair(state >> 6, state & 63);
            if ((visited[q] >> r) & 1) return false;

            int target = (carry << 1) | ((n >> i) & 1);
            if (target <= l + 1) {
                for (int m : masks[index(l + 1, target)]) {
                    temp = a;
                    for (int j = 0; j < l; j++)
                        if ((m >> j) & 1) {
                            if (!temp[j] || (temp[j] < 0 && ((i + 1) & 1))) goto next;

                            if (temp[j] > 0) temp[j]--;
                            else temp[j] = i / 2;
                        }

                    if (self(self, i - 1, (m >> l) & 1, temp)) {
                        for (int j = 0; j < l; j++)
                            if ((m >> j) & 1) values[j] |= 1LL << i;
                        return true;
                    }
                    next:;
                }
                if (l == 3 && carry && ~(n >> i) & 1 && self(self, i - 1, 2, a)) return true;
            }

            if (!visited[q]) st.emplace(q);
            visited[q] |= 1LL << r;
            return false;
        };

        if (!dfs(dfs, w - 1)) cout << "Impossible\n";
        else {
            for (auto v : values) cout << v << " ";
            while (k-- > l) cout << "0 ";
            cout << "\n";
        }

        while (!st.empty()) {
            visited[st.top()] = 0;
            st.pop();
        }
    }
}
