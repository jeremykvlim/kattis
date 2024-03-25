#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<long long> pref(n + 1), dp(n + 1), prev(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> pref[i];
        
        pref[i] += pref[i - 1];
    }

    for (int i = 1; i <= n; i++)
        for (int j = i - 1; ~j; j--) 
            if (prev[j] <= pref[i] - pref[j]) {
                dp[i] = dp[j] + 1;
                prev[i] = pref[i] - pref[j];
                break;
            }
        

    cout << dp[n];
}
