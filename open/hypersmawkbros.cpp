#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    vector<bool> cases(t);
    vector<array<int, 3>> queries;
    for (int i = 0; i < t; i++) {
        int n, m;
        cin >> n >> m;

        if (!(m & 1)) cases[i] = !!(n % (m + 1));
        else queries.push_back({m, n, i});
    }
    sort(queries.begin(), queries.end());

    for (int q = 0; q < queries.size();) {
        int m = queries[q][0];

        vector<int> losing{0, m + 1};
        for (; q < queries.size() && queries[q][0] == m; q++) {
            auto [m, n, i] = queries[q];
            while (losing.back() < n) {
                auto win = [&](auto &&self, int health, int damage, int k) -> bool {
                    if (damage > m) return true;
                    if (!k) return (health != m || damage != m);

                    if (!((health - losing[k]) & 1))
                        if (!self(self, (health + losing[k]) / 2, (health - losing[k]) / 2, k)) return true;

                    if (!((health - losing[k - 1]) & 1))
                        if (!self(self, (health + losing[k - 1]) / 2, (health - losing[k - 1]) / 2, k - 1)) return true;

                    if (losing[k] - losing[k - 1] == m + 2)
                        if (health == losing[k] + (m - 1) / 2 && damage != (m + 1) / 2) return true;

                    return false;
                };
                int health = losing.back() + m + 1;
                if (win(win, health, 0, losing.size() - 1)) losing.emplace_back(health + 1);
                else losing.emplace_back(health);
            }
            cases[i] = n != losing.back();
        }
    }
    for (bool b : cases) cout << (b ? "YES\n" : "NO\n");
}