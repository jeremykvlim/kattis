#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int sum_s = 0, sum_p = 0;
    vector<pair<int, int>> trades(n);
    for (auto &[s, p] : trades) {
        cin >> s >> p;

        sum_s += s;
        sum_p += p;
    }
    trades.emplace_back(-sum_s, -sum_p);
    sort(trades.begin(), trades.end(), [](auto p1, auto p2) { return atan2(p1.first, p1.second) > atan2(p2.first, p2.second); });

    int j = -1;
    for (int i = 0; i <= n; i++) {
        auto [s, p] = trades[i];
        if (s == -sum_s && p == -sum_p) {
            j = i;
            break;
        }
    }

    sum_s = 0;
    auto profit = 0LL;
    for (int _ = 0, i = (j + 1) % (n + 1); _ < n; _++, ++i %= (n + 1)) {
        auto [s, p] = trades[i];
        sum_s += s;
        profit += (long long) sum_s * p;
    }
    cout << profit;
}
