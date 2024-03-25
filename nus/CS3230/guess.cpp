#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int l = 0, r = 1001, m;
    while (l + 1 < r) {
        m = l + (r - l) / 2;
        cout << m << "\n" << flush;

        string s;
        cin >> s;

        if (s == "lower") r = m;
        else if (s == "higher") l = m;
        else break;
    }
}
