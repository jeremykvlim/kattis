#include <bits/stdc++.h>
using namespace std;

pair<long long, long long> bezout(long long a, long long b) {
    if (!a) return {0, 1};
    auto [x, y] = bezout(b % a, a);
    return {y - (b / a) * x, x};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    while (t--) {
        long long R, S, Q;
        cin >> R >> S >> Q;
        
        auto [A, B] = bezout(R, -S);
        long long l = INT_MIN, r = INT_MAX, m;
        while (l + 1 < r) {
            m = l + (r - l) / 2;
            if (Q / __gcd(R, -S) * A + m * S / __gcd(R, -S) > 0 && Q / __gcd(R, -S) * -B - m * R / __gcd(R, -S) > 0) l = m;
            else r = m;
        }
        cout << Q / __gcd(R, -S) * A + l * S / __gcd(R, -S) << " " << Q / __gcd(R, -S) * -B - l * R / __gcd(R, -S) << "\n";
    }
}
