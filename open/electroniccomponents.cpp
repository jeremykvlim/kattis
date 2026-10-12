#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, long long>> components(n);
    int most = 0;
    for (auto &[f, t] : components) {
        cin >> f >> t;

        most = max(most, f);
    }
    sort(components.begin(), components.end(), [&](auto p1, auto p2) { return p1.second > p2.second; });

    vector<long long> dp(2 * most + 1, 1e18), temp(2 * most + 1);
    dp[0] = 0;
    for (int parity = 0; auto [f, t] : components) {
        fill(temp.begin(), temp.end(), 1e18);

        auto least = LLONG_MAX;
        for (int i = parity ^ (f & 1); i < f; i += 2) {
            int l = f - i, r = f + i;
            least = min({least, 2 * dp[l] - t * l, 2 * dp[r] - t * r});
            temp[i] = (t * (f + i) + least) / 2;
        }

        auto value = [&](int i) {
            return 2 * dp[i] - t * i;
        };

        deque<int> dq;
        for (int i = f + parity, j = parity - 2; i <= 2 * most; i += 2) {
            int r = min(2 * most, f + i);
            while (j + 2 <= r) {
                j += 2;
                while (!dq.empty() && value(dq.back()) >= value(j)) dq.pop_back();
                dq.emplace_back(j);
            }
            while (f - i < -dq.front()) dq.pop_front();
            temp[i] = (t * (f + i) + value(dq.front())) / 2;
        }
        dp = temp;
        parity ^= f & 1;
    }
    cout << *min_element(dp.begin(), dp.end());
}