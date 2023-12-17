#include <bits/stdc++.h>
using namespace std;

int swaps(vector<int> &ally, int t) {
    auto pos = upper_bound(ally.begin(), ally.end(), t) - ally.begin();
    return min((pos < ally.size() ? ally[pos] - t : INT_MAX), (pos ? t - ally[pos - 1] : INT_MAX));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    string s;
    cin >> n >> s;

    int favor = 0;
    vector<int> tellers, signums = {0};
    for (char c : s)
        if (c == '0') tellers.emplace_back(signums.size() - 1);
        else {
            favor += c == '1' ? 1 : -1;
            signums.emplace_back((favor > 0) - (favor < 0));
        }

    vector<vector<int>> allies(4);
    for (int i = 0; i < signums.size(); i++) {
        if (!signums[i]) allies[0].emplace_back(i);
        else if (signums[i] == 1) allies[1].emplace_back(i);
    }

    int required = max(1 - accumulate(tellers.begin(), tellers.end(), 0, [&](int i, int j) {return i + signums[j];}), 0);

    vector<long long> dp(required + 2, INT_MAX);
    dp[0] = dp[required + 1] = 0;
    for (int t : tellers) {
        pair<long long, long long> costs = {swaps(allies[signums[t] + 1], t), swaps(allies[signums[t] + 2], t)};
        for (int i = required; i; i--) dp[i] = min({dp[i], dp[i - 1] + costs.first, (i > 1 ? dp[i - 2] + costs.second : INT_MAX)});
    }

    cout << (dp[required] < INT_MAX ? to_string(dp[required]) : "impossible");
}
