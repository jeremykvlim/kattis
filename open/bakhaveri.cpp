#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> count(1 << m, 0);
    while (n--) {
        int k;
        cin >> k;

        int mask = 0;
        while (k--) {
            int a;
            cin >> a;

            mask |= 1 << (a - 1);
        }
        count[mask]++;
    }

    vector<vector<int>> masks_by_p(m + 1);
    for (auto mask = 1U; mask < 1 << m; mask++) masks_by_p[popcount(mask)].emplace_back(mask);

    int pastries = 0;
    vector<int> dp1(1 << m, -1), dp2(1 << m, -1);
    dp2[0] = 0;
    for (int p = 1; p <= m; p++)
        for (int m1 : masks_by_p[p]) {
            for (int i = 0; i < m; i++)
                if ((m1 >> i) & 1) {
                    int m2 = m1 ^ (1 << i);
                    dp1[m1] = max({dp2[m2], dp1[m1], dp1[m2]});
                }
            pastries = max(pastries, dp2[m1] = count[m1] + dp1[m1]);
        }
    cout << pastries;
}
