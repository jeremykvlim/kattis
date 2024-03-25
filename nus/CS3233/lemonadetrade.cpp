#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    unordered_map<string, double> dp{{"pink", 0}};
    while (n--) {
        string o, w;
        double r;
        cin >> o >> w >> r;

        if (dp.count(w)) dp[o] = dp.count(o) ? max(dp[o], log(r) + dp[w]) : log(r) + dp[w];
    }

    cout << fixed << setprecision(6) << (dp.count("blue") ? min(10.0, exp(dp["blue"])) : 0);
}
