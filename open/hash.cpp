#include <bits/stdc++.h>
using namespace std;

void f(int state, int size, vector<int> &count, int mod, long long &words, long long multiplier = 33, bool second = false) {
    if (size)
        for (int ord = 1; ord <= 26; ord++)
            f((second ? ((state ^ ord) * multiplier) & mod : (state * multiplier) ^ ord) & mod,
              size - 1, count, mod, words, multiplier, second);
    else {
        if (second) words += count[state];
        else count[state]++;
    }
}

long long mul(long long x, long long y, long long mod) {
    auto product = x * y - mod * (long long) (1.L / mod * x * y);
    return product + mod * (product < 0) - mod * (product >= mod);
}

long long pow(long long base, long long exponent, long long mod = LLONG_MAX) {
    auto value = 1LL;
    for (; exponent; exponent >>= 1) {
        if (exponent & 1) value = mul(value, base, mod);
        base = mul(base, base, mod);
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
    auto x = pow(33, (mod >> 1) - 1, mod), words = 0LL;

    f(0, n / 2, count, mod - 1, words);
    f(k, n - n / 2, count, mod - 1, words, x, true);
    cout << words;
}
