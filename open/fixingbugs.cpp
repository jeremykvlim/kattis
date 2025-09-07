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
    vector<vector<vector<double>>> memo(1 << b, vector<vector<double>>(t, vector<double>(t, 0)));
    vector<vector<vector<bool>>> visited(1 << b, vector<vector<bool>>(t, vector<bool>(t, false)));
    auto dp = [&](auto &&self, int mask = 0, int time = 0, int count = 0) -> double {
        if (mask == (1 << b) - 1 || time == t) return 0.;
        if (visited[mask][time][count]) return memo[mask][time][count];

        visited[mask][time][count] = true;
        double prob = -1;
        int j;
        for (int i = 0; i < b; i++)
            if (!((mask >> i) & 1) && p[i] * s[i] > prob) {
                prob = p[i] * s[i];
                j = i;
            }

        auto temp = p[j];

        p[j] *= f;
        fails[j]++;
        memo[mask][time][count] += (1. - temp) * self(self, mask, time + 1, count + 1);

        p[j] = temp;
        fails[j]--;
        memo[mask][time][count] += p[j] * (s[j] + self(self, mask | (1 << j), time + 1, count - fails[j]));

        return memo[mask][time][count];
    };
    cout << fixed << setprecision(6) << dp(dp);
}
