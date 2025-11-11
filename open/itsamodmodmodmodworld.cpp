#include <bits/stdc++.h>
using namespace std;

long long floor_division_sum(long long n, int a, int c, int d) {
    auto sum = 0LL;
    while (n) {
        if (a >= d) {
            sum += (a / d) * (__int128) (n * (n - 1)) / 2;
            a %= d;
        }
        if (c >= d) {
            sum += n * (c / d);
            c %= d;
        }

        auto y_max = a * n + c;
        if (y_max < d) break;
        n = y_max / d;
        c = y_max % d;
        swap(a, d);
    }
    return sum;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int w;
    cin >> w;

    while (w--) {
        long long p, q, n;
        cin >> p >> q >> n;
        cout << p * n * (n + 1) / 2 - q * floor_division_sum(n + 1, p, 0, q) << "\n";
    }
}
