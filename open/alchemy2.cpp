#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    int n = s.size();
    pair<int, int> dp{0, n + 1};
    for (int i = 0; i < n / 2; i++) {
        if (s[i] != s[n - i - 1]) dp = {dp.second, min(dp.first, dp.second) + 1};
        else dp = {min(dp.first, dp.second + 1), dp.second + 1};
    }

    cout << min(dp.first, dp.second);
}
