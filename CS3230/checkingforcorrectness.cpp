#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e4;

long long pow(long long base, long long exponent, long long mod = 1) {
    auto value = 1LL;
    for (; exponent; exponent >>= 1) {
        if (exponent & 1) value = (base * value) % mod;
        base = (base * base) % mod;
    }
    return value;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long a, b;
    char op;

    while (cin >> a >> op >> b) {
        a %= MODULO;

        if (op == '^') cout << pow(a, b, MODULO) << "\n"; 
        else {
            b %= MODULO;
            cout << (op == '+' ? (a + b) % MODULO : (a * b) % MODULO) << "\n";
        }
    }
}
