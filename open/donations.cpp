#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long x, y, z, c, r;
    cin >> n >> x >> y >> z >> c >> r;

    int limit = z / y;
    auto credit = c / 100.L, interest = 1 + r / 100.L;
    vector<long double> dp(n + 1, -1e20), temp(n + 1);
    dp[0] = x;
    for (int i = 0; i < n; i++) {
        fill(temp.begin(), temp.end(), -1e20);
        deque<int> mono;
        auto most = -1e20L;
        for (int j = i; j <= n; j++) {
            int k = j - limit - 1;
            if (k >= i) most = max(most, dp[k] + (1 - credit) * k * y);
            while (!mono.empty() && mono.front() < j - limit) mono.pop_front();

            auto value = dp[j] + j * y;
            while (!mono.empty() && dp[mono.back()] + mono.back() * y <= value) mono.pop_back();
            mono.emplace_back(j);

            auto curr = dp[mono.front()] + (mono.front() - j) * y;
            if (most != -1e20) curr = max(curr, most - (1 - credit) * j * y - credit * z);
            temp[j] = curr * interest;
        }
        dp = temp;
    }
    cout << fixed << setprecision(6) << dp[n];
}