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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, m, mod;
    cin >> n >> k >> m;
    mod = (1 << m);
    vector<int> count(mod);
    long long a = 33, x = 1, words = 0;
    for (int exponent = (mod >> 1) - 1; exponent; exponent >>= 1) {
        if (exponent & 1) x = (a * x) % mod;
        a = (a * a) % mod;
    }
    mod--;
    f(0, n / 2, count, mod, words);
    f(k, n - n / 2, count, mod, words, x, true);
    cout << words;
}
