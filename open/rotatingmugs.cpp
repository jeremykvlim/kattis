#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m;
    string s;
    cin >> m >> s;

    int sum = 0, r_max = -1;
    for (char c : s) {
        int d;
        if (c == 'S') d = 0;
        else if (c == 'E') d = 1;
        else if (c == 'N') d = 2;
        else d = 3;

        int r = (6 - d) % 4;
        r_max = max(r_max, r);
        sum += r;
    }

    if (m == 2) cout << (s[0] == s[1] ? sum / 2 : -1);
    else if (sum & 1) cout << -1;
    else if (!sum) cout << 0;
    else if (r_max <= sum / 2) cout << sum / 2;
    else cout << 4 + (r_max == 2);
}
