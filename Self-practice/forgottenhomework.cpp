#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e9 + 7;

long long pow(long long base, long long exponent, long long mod = MODULO) {
    auto value = 1LL;
    for (; exponent; exponent >>= 1) {
        if (exponent & 1) value = (base * value) % mod;
        base = (base * base) % mod;
    }
    return value;
}

vector<long long> berlekamp_massey(vector<long long> sequence) {
    vector<long long> curr(sequence.size()), prev(sequence.size());
    curr[0] = prev[0] = 1;
    int len = 0;

    auto base = 1LL;
    for (int i = 0, count = 0; i < sequence.size(); i++) {
        count++;
        auto discrepancy = 0LL;
        for (int j = 0; j <= len; j++) discrepancy = (discrepancy + curr[j] * sequence[i - j]) % MODULO;
        if (!discrepancy) continue;

        long long coeff = (discrepancy * pow(base, (long long) MODULO - 2)) % MODULO;
        auto temp = curr;
        for (int j = count; j < sequence.size(); j++) curr[j] = (curr[j] - coeff * prev[j - count]) % MODULO;
        if (2 * len <= i) {
            len = i + 1 - len;
            prev = temp;
            base = discrepancy;
            count = 0;
        }
    }

    curr.assign(curr.begin() + 1, curr.begin() + len + 1);
    for (auto &c : curr) c = (MODULO - c) % MODULO;
    return curr;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, i, j;
    cin >> n >> i >> j;

    vector<long long> sequence(2 * n - 1);
    for (auto &a : sequence) cin >> a;
    sequence.insert(sequence.begin(), i != j ? 0 : 1);

    auto lfsr = berlekamp_massey(sequence);
    auto value = 0LL;
    for (int k = 0; k < lfsr.size(); k++) value += sequence[2 * n - 1 - k] * lfsr[k] % MODULO;
    cout << (value + MODULO) % MODULO;
}
