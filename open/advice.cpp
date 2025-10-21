#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, t, m;
    string S;
    cin >> n >> t >> m >> S;

    vector<vector<int>> pref(26, vector<int>(n + 1));
    for (int c = 0; c < 26; c++)
        for (int i = 0; i < n; i++) pref[c][i + 1] = pref[c][i] + ((S[i] - 'A' + c) % 26);

    int l = 0, shift = 0;
    for (; t--; ++shift %= 26) {
        int len = n - l;
        l += ((len + pref[shift][n] - pref[shift][l]) % m) % len;
    }

    string s;
    for (int i = l; i < n; i++) s += 'A' + (S[i] - 'A' + shift) % 26;
    cout << s;
}
