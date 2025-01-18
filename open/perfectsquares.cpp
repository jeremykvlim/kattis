#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;

    int a = 0;
    while (!(n % 4)) {
        n /= 4;
        a++;
    }

    if (n % 8 == 7) {
        cout << -1;
        exit(0);
    }
    
    for (auto z = 0LL; z * z <= n; z++) {
        auto remaining = n - z * z;
        for (auto y = 0LL; y * y <= remaining; y++) {
            long long x = sqrt(remaining - y * y);

            if (x * x + y * y == remaining) {
                cout << x * (1 << a) << " " << y * (1 << a) << " " << z * (1 << a);
                exit(0);
            }
        }
    }
}
