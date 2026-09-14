#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int r = 224 * 5 - 9, l = r - (1 << 10), m, elapsed = 0;
    while (l + 1 < r) {
        m = l + (r - l) / 2;

        cout << "? " << m - elapsed << "\n" << flush;

        string s;
        getline(cin, s);

        if (s.find("gerðist") != string::npos) exit(0);

        elapsed += 9;
        if (s == "Já") r = m;
        else l = m;
    }
    cout << "! " << r - elapsed;
}