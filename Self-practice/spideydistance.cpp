#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long t, s;
    cin >> t >> s;

    auto n = 2 * min(t, s) * (min(t, s) + 1) + 1, d = 2 * s * (s + 1) + 1;
    for (int i = s - 2, j = max(0, (int) t - (int) s); i > 0; i -= 3) {
        d += 4 * i;
        if (j) {
            n += 4 * i;
            j--;
        }
    }

    auto g = __gcd(n, d);
    cout << n / g;
    if (d != g) cout << "/" << d / g;
}
