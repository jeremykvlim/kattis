#include <bits/stdc++.h>
using namespace std;

double fix(int t, double f, vector<double> &p, vector<int> &s, vector<int> &fails, vector<vector<int>> &bug, vector<vector<vector<double>>> &dp, int open, int count) {
    if (!t || !open) return 0;

    if (!dp[open][t][count]) {
        int &curr = bug[open][count];
        if (curr == -1) {
            double value = -1;
            for (int i = 0; i < p.size(); i++)
                if ((open & (1 << i)) && p[i] * s[i] > value) {
                    curr = i;
                    value = p[i] * s[i];
                }
        }

        auto prob = p[curr];
        p[curr] *= f;
        fails[curr]++;
        dp[open][t][count] += (1 - prob) * fix(t - 1, f, p, s, fails, bug, dp, open, count + 1) + 1;

        p[curr] = prob;
        fails[curr]--;
        dp[open][t][count] += prob * (s[curr] + fix(t - 1, f, p, s, fails, bug, dp, open & ~(1 << curr), count - fails[curr]));
    }

    return dp[open][t][count] - 1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int b, t;
    double f;
    cin >> b >> t >> f;

    vector<double> p(b);
    vector<int> s(b), fails(b);
    for (int i = 0; i < b; i++) cin >> p[i] >> s[i];

    vector<vector<int>> bug(1 << b, vector<int>(t + 1, -1));
    vector<vector<vector<double>>> dp(1 << b, vector<vector<double>>(t + 1, vector<double>(t + 1, 0)));
    
    cout << fixed << setprecision(6) << fix(t, f, p, s, fails, bug, dp, (1 << b) - 1, 0);
}
