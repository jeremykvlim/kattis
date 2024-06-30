#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n, a, b;
    cin >> n >> a >> b;

    auto g = __gcd(a, b);
    a /= g;
    b /= g;

    long long slice;
    if ((double) n / a / b > 1) {
        slice = sqrt((double) n / a / b);
        while (slice * a * slice * b < n) slice++;
        a *= slice;
        b *= slice;
    }

    int cuts = 0;
    while (a > 1 || b > 1) {
        if (b > a) swap(a, b);
        a = (a + 1) / 2;
        cuts++;
    }

    cout << cuts;
}
