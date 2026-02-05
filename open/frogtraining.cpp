#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    getline(cin, s);

    istringstream iss(s);
    vector<int> x{istream_iterator<int>{iss}, istream_iterator<int>{}};

    int h;
    cin >> h;

    vector<long long> dp(h + 1, 0);
    dp[0] = 1;
    for (int xi : x)
        for (int i = 0; i + xi <= h; i++) dp[i + xi] += dp[i];
    cout << dp[h];
}
