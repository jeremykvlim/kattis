#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long long m;
    cin >> n >> m;

    for (auto q = 2LL;; q++) {
        auto q_pow = 1LL;
        for (int _ = 0; _ < n - 1; _++) {
            if (q_pow > m / q) {
                cout << "impossible";
                exit(0);
            }
            q_pow *= q;
        }

        for (auto t = q - 1; t; t--) {
            auto s = 0LL, a = q_pow;
            
            for (int _ = 0; _ < n; _++) {
                s += a;
                a /= q;
                a *= t;
                if (s > m) goto next;
            }

            if (!(m % s)) {
                auto p = q - t;
                cout << p << " " << q;
                exit(0);
            }
            next:;
        }
    }
}
