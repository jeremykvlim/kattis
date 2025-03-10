#include <bits/stdc++.h>
using namespace std;

template <typename T, typename U, typename V>
T mul(U x, V y, T mod) {
    return (unsigned __int128) x * y % mod;
}

template <typename T, typename U>
T pow(T base, U exponent, T mod) {
    T value = 1;
    while (exponent) {
        if (exponent & 1) value = mul(base, value, mod);
        base = mul(base, base, mod);
        exponent >>= 1;
    }
    return value;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, m;
    cin >> n >> k >> m;

    int mod = 1 << m;
    vector<int> count(mod);
    int x = pow(33, (mod >> 1) - 1, mod), words = 0;

    auto f = [&](auto &&self, int state, int size, int mul = 33, bool reverse = false) -> void {
        if (size)
            for (int ord = 1; ord <= 26; ord++)
                self(self, (reverse ? (state ^ ord) * mul : (state * mul) ^ ord) & (mod - 1), size - 1, mul, reverse);
        else {
            if (reverse) words += count[state];
            else count[state]++;
        }
    };

    f(f, 0, n / 2);
    f(f, k, n - n / 2, x, true);
    cout << words;
}
