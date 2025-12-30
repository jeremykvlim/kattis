#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    int k = bit_ceil((unsigned) m);
    vector<int> slots(k);
    iota(slots.begin(), slots.end(), n + m + 1);

    int u = slots[k - 1] + 1;
    vector<pair<int, int>> pipes;
    vector<vector<int>> dp(2 * k);
    for (int i = 0; i < k; i++) dp[i + k].emplace_back(slots[i]);

    auto merge = [&](const vector<int> &l, const vector<int> &r) -> vector<int> {
        if (l.empty() || r.empty()) return {};
        vector<int> p;
        for (int i = 0; i < l.size(); i++) {
            int v = u++;
            pipes.emplace_back(l[i], v);
            pipes.emplace_back(r[i], v);
            p.emplace_back(u++);
            pipes.emplace_back(v, p.back());
            p.emplace_back(u++);
            pipes.emplace_back(v, p.back());
        }
        return p;
    };
    for (int i = k - 1; i; i--) dp[i] = merge(dp[i << 1], dp[i << 1 | 1]);
    for (int i = 0; i < n; i++) pipes.emplace_back(i + 1, slots[i]);
    for (int i = 0; i < m; i++) pipes.emplace_back(dp[1][i], i + n + 1);
    for (int i = m; i < k; i++) pipes.emplace_back(dp[1][i], slots[i]);
    cout << u - 1 << " " << pipes.size() << "\n";
    for (auto [a, b] : pipes) cout << a << " " << b << "\n";
}
