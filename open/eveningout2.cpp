#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long a, b;
    cin >> a >> b;

    if (a < b) cout << b - a;
    else {
        auto coins = a;
        for (int p = 1; p <= sqrt(a); p <= 2 ? p++ : p += 2)
            if (!(a % p)) coins = min(coins, min(abs(b - p), abs(b - a / p)));

        cout << coins;
    }
}
