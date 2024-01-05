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
    vector<long long> curr, prev;
    for (int i = 0, len = -1; i < sequence.size(); i++) {
        auto discrepancy = sequence[i];
        for (int j = 1; j <= curr.size(); j++) discrepancy = (discrepancy - curr[j - 1] * sequence[i - j]) % MODULO;
        if (!discrepancy) continue;

        if (len == -1) {
            curr.resize(i + 1);
            len = i;
            continue;
        }

        auto temp1 = prev;
        for (auto &c : temp1) c = (MODULO - c) % MODULO;
        temp1.insert(temp1.begin(), 1);

        auto base = 0LL;
        for (int j = 1; j <= temp1.size(); j++) base = (base + temp1[j - 1] * sequence[len + 1 - j]) % MODULO;
        long long coeff = (discrepancy * pow(base, (long long) MODULO - 2)) % MODULO;
        for (auto &c : temp1) c = (c * coeff) % MODULO;

        temp1.insert(temp1.begin(), i - len - 1, 0);
        auto temp2 = curr;
        curr.resize(max(curr.size(), temp1.size()));
        for (int j = 0; j < temp1.size(); j++) curr[j] = (curr[j] + temp1[j]) % MODULO;
        if (i - temp2.size() > len - prev.size()) {
            prev = temp2;
            len = i;
        }
    }

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
    for (int k = 0; k < lfsr.size(); k++) value = (value + sequence[2 * n - 1 - k] * lfsr[k]) % MODULO;
    cout << (value + MODULO) % MODULO;
}
