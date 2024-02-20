#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        long long n, d;
        cin >> n >> d;

        int sites = 0;
        for (auto i = 1LL; i < n; i *= d + 1) {
            sites++;
            if (i > n / (d + 1)) break;
        }

        cout << sites << "\n";
    }
}
