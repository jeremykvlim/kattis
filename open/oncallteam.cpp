#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    int k = 1 << m;
    vector<int> count(k, 0);
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;

        int mask = 0;
        for (int j = 0; j < m; j++)
            if (s[j] == '1') mask |= 1 << j;
        count[mask]++;
    }

    for (int b = 0; b < m; b++)
        for (int mask = 0; mask < k; mask++)
            if ((mask >> b) & 1) count[mask] += count[mask ^ (1 << b)];

    int robustness = m + 1;
    for (auto mask = 1U; mask < k; mask++) {
        int p = popcount(mask);
        if (n - count[(k - 1) ^ mask] < p) robustness = min(robustness, p);
    }
    cout << robustness - 1;
}
