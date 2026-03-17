#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int l = 0, r = 31, m, q = 24;
    while (l + 1 < r && q--) {
        m = l + (r - l) / 2;

        cout << "? " << (1 << (m - 1)) << "\n" << flush;
        string line;
        getline(cin, line);

        if (line == "Too low!") l = m;
        else if (line == "Too high!") r = m;
        else exit(0);
    }

    double left = !l ? 0 : 1 << (l - 1), right = !l ? 1 : min(left * 2, 1e9), mid;
    while (q--) {
        mid = left + (right - left) / 2;

        cout << fixed << setprecision(8) << "? " << mid << "\n" << flush;
        string line;
        getline(cin, line);

        if (line == "Too low!") left = mid;
        else if (line == "Too high!") right = mid;
        else exit(0);
    }
}
