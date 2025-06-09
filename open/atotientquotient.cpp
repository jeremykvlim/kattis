#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int a, b;
    cin >> a >> b;

    map<int, int> count;
    for (int p = 2; p <= 1e4; p == 2 ? p++ : p += 2) {
        while (!(a % p)) {
            a /= p;
            count[p]++;
        }
        while (!(b % p)) {
            b /= p;
            count[p]--;
        }
    }

    auto m = 1LL, n = 1LL;
    while (!count.empty()) {
        auto [p, c] = *prev(count.end());
        count.erase(prev(count.end()));

        if (!c) continue;

        if (c & 1) {
            (c < 0 ? n : m) *= pow(p, (abs(c) + 1) / 2);
            int q = p - 1;
            for (int P = 2; P * P <= q; P == 2 ? P++ : P += 2)
                while (!(q % P)) {
                    q /= P;
                    c < 0 ? count[P]++ : count[P]--;
                }

            if (q > 1) c < 0 ? count[q]++ : count[q]--;
        } else {
            (c < 0 ? n : m) *= pow(p, abs(c) / 2 + 1);
            (c < 0 ? m : n) *= p;
        }
    }
    cout << m << " " << n;
}
