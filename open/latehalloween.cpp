#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int sum = 0;
    vector<int> c(n);
    for (int &ci : c) {
        cin >> ci;

        sum += ci;
    }

    if (sum % 3) {
        cout << "False";
        exit(0);
    }

    sum /= 3;
    vector<bitset<(int) 1e3 + 1>> dp(sum + 1);
    dp[0][0] = true;
    for (int ci : c)
        for (int s = sum; ~s; s--)
            if (dp[s].any()) {
                if (s + ci <= sum) dp[s + ci] |= dp[s];
                dp[s] |= dp[s] << ci;
            }

    cout << (dp[sum][sum] ? "True" : "False");
}
