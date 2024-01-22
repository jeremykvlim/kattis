#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e4;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long a, b;
    char op;

    while (cin >> a >> op >> b) {
        a %= MODULO;

        if (op == '^') {
            auto ans = 1LL;
            for (; b; b >>= 1) {
                if (b & 1) ans = (ans * a) % MODULO;
                a = (a * a) % MODULO;
            }

            cout << ans << "\n";
        }
        else {
            b %= MODULO;
            cout << (op == '+' ? (a + b) % MODULO : (a * b) % MODULO) << "\n";
        }
    }
}
