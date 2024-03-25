#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;

    while (t--) {
        double r, b, m;
        cin >> r >> b >> m;

        int payments = 0;
        while (b > 1e-3 && payments < 1200) {
            b += round(r / 100 * b * 100) / 100;
            b = round(b * 100) / 100 - m;
            payments++;
        }

        cout << (b > 1e-3 ? "impossible" : to_string(payments)) << "\n";
    }
}
