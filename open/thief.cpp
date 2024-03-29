#include <bits/stdc++.h>
using namespace std;

void dnc(int l, int r, int lower, int upper, int offset, int size, vector<long long> &total, vector<long long> &dp, vector<long long> &temp) {
    if (l > r) return;

    int m = l + (r - l) / 2;
    auto p = make_pair(LLONG_MIN, 0);
    for (int i = lower; i <= min(m, upper); i++) p = max(p, {total[(m - i) * size] + dp[i * size + offset], i});
    temp[m * size + offset] = p.first;

    dnc(l, m - 1, lower, p.second, offset, size, total, dp, temp);
    dnc(m + 1, r, p.second, upper, offset, size, total, dp, temp);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<vector<int>> jewels(301);
    for (int i = 1; i <= n; i++) {
        int s, v;
        cin >> s >> v;

        jewels[s].emplace_back(v);
    }
    for (auto &j : jewels) sort(j.rbegin(), j.rend());

    vector<long long> total(k + 1, 0), dp(k + 1), temp(k + 1);
    for (int i = 1; i < jewels.size(); i++) {
        if (jewels[i].empty()) continue;

        auto sum = 0LL;
        for (int j = 0; i * (j + 1) <= k; j++) {
            if (j < jewels[i].size()) sum += jewels[i][j];
            total[i * (j + 1)] = sum;
        }
        if (!sum) break;

        for (int j = 0; j < i; j++) dnc(0, (k - j) / i, 0, (k - j) / i, j, i, total, dp, temp);
        dp = temp;
    }

    for (int i = 1; i <= k; i++) cout << dp[i] << " ";
}
