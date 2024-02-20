#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    double d;
    int n;
    cin >> d >> n;

    int total = 0;
    double l = 0, r = 1;
    for (int i = 1; i <= n; i++) {
        int c;
        cin >> c;

        total += c;
        l = max(l, d * i - total);
        r = min(r, d * i - total + 1);
    }

    cout << ((l < r) ? "possible" : "impossible");
}
