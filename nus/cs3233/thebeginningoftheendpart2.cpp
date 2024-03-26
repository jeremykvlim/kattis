#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    string s;
    cin >> n >> s;

    if (s[0] != 'G') {
        cout << "IMPOSSIBLE";
        exit(0);
    }

    cout << "2 2\n";
    for (int i = 1, outcome, offset = 0; i < n; i++, offset = outcome - 2) {
        outcome = (s[i] == 'G') ? 1 : (s[i] == 'B') ? -1 : 0;
        cout << 2 + outcome - offset << " " << 6 - (outcome - offset) << "\n";
    }
}
