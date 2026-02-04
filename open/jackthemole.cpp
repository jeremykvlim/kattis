#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int sum = 0;
    vector<int> w(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> w[i];

        sum += w[i];
    }

    vector<int> suspects;
    for (int i = 1; i <= n; i++) {
        int remaining = sum - w[i];
        if (remaining & 1) continue;

        bitset<150001> dp;
        dp[0] = true;
        for (int j = 1; j <= n; j++)
            if (i != j) dp |= (dp << w[j]);

        if (dp[remaining / 2]) suspects.emplace_back(i);
    }
    cout << suspects.size() << "\n";
    for (int s : suspects) cout << s << " ";
}
