#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int b, t;
    double f;
    cin >> b >> t >> f;

    vector<double> p(b), s(b);
    for (int i = 0; i < b; i++) cin >> p[i] >> s[i];

    vector<int> fails(b, 0);
    vector<vector<vector<double>>> dp(1 << b, vector<vector<double>>(t, vector<double>(100, 0)));
    vector<vector<vector<bool>>> visited(1 << b, vector<vector<bool>>(t, vector<bool>(100, false)));
    auto fix = [&](auto &&self, int mask = 0, int time = 0, int count = 0) -> double {
        if (mask == (1 << b) - 1 || time == t) return 0.;
        if (visited[mask][time][count]) return dp[mask][time][count];

        visited[mask][time][count] = true;
        double prob = -1.0;
        int j;
        for (int i = 0; i < b; i++)
            if (!(mask & (1 << i)) && p[i] * s[i] > prob) {
                prob = p[i] * s[i];
                j = i;
            }

        auto temp = p[j];

        p[j] *= f;
        fails[j]++;
        dp[mask][time][count] += (1. - temp) * self(self, mask, time + 1, count + 1);

        p[j] = temp;
        fails[j]--;
        dp[mask][time][count] += p[j] * (s[j] + self(self, mask | (1 << j), time + 1, count - fails[j]));

        return dp[mask][time][count];
    };

    cout << fixed << setprecision(6) << fix(fix);
}
