#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    long long d1, d2, d3, d4, d5, x, y;
    while (n--) {
        cout << "0 0\n" << flush;
        cin >> d1;
        if (!d1) continue;

        cout << "1 0\n" << flush;
        cin >> d2;
        if (!d2) continue;

        cout << "0 1\n" << flush;
        cin >> d3;
        if (!d3) continue;

        x = (d1 - d2 + 1) / 2;
        cout << x << " " << (long long) sqrt(d1 - x * x) << "\n" << flush;
        cin >> d4;
        if (!d4) continue;

        y = (d1 - d3 + 1) / 2;
        cout << (long long) sqrt(d1 - y * y) << " " << y << "\n" << flush;
        cin >> d5;
    }
}
