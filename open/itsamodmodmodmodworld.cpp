#include <bits/stdc++.h>
using namespace std;

long long sum(long long p, long long q, long long n) {
    if (!n || !p) return 0;
    if (n >= q) return (n / q) * p * (n + 1) - (n / q) * ((n / q) * p * q + p + q - 1) / 2 + sum(p, q, n % q);
    if (p >= q) return (p / q) * n * (n + 1) / 2 + sum(p % q, q, n);
    return ((n * p) / q) * n - sum(q, p, n * p / q);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int w;
    cin >> w;

    while (w--) {
        long long p, q, n;
        cin >> p >> q >> n;

        auto g = __gcd(p, q);
        cout << p * n * (n + 1) / 2 - q * sum(p / g, q / g, n) << "\n";
    }
}
