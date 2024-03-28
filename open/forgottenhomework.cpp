#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e9 + 7;

long long pow(long long base, long long exponent, long long mod = LLONG_MAX) {
    auto value = 1LL;
    for (; exponent; exponent >>= 1) {
        if (exponent & 1) value = (base * value) % mod;
        base = (base * base) % mod;
    }
    return value;
}

vector<long long> berlekamp_massey(vector<long long> seq) {
    vector<long long> curr, prev;
    for (int i = 0, len = -1; i < seq.size(); i++) {
        auto discrepancy = seq[i];
        for (int j = 1; j <= curr.size(); j++) discrepancy = (discrepancy - curr[j - 1] * seq[i - j]) % MODULO;
        if (!discrepancy) continue;

        if (len == -1) {
            curr.resize(i + 1);
            len = i;
            continue;
        }

        auto temp1 = prev;
        for (auto &c : prev) c = (MODULO - c) % MODULO;
        prev.insert(prev.begin(), 1);

        auto base = 0LL;
        for (int j = 1; j <= prev.size(); j++) base = (base + prev[j - 1] * seq[len + 1 - j]) % MODULO;
        auto coeff = (discrepancy * pow(base, MODULO - 2, MODULO)) % MODULO;
        for (auto &c : prev) c = (c * coeff) % MODULO;

        prev.insert(prev.begin(), i - len - 1, 0);
        auto temp2 = curr;
        curr.resize(max(curr.size(), prev.size()));
        for (int j = 0; j < prev.size(); j++) curr[j] = (curr[j] + prev[j]) % MODULO;
        if (i - temp2.size() > len - temp1.size()) {
            prev = temp2;
            len = i;
        } else prev = temp1;
    }

    return curr;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, i, j;
    cin >> n >> i >> j;

    vector<long long> seq(2 * n - 1);
    for (auto &a : seq) cin >> a;
    seq.emplace(seq.begin(), i == j);

    auto lfsr = berlekamp_massey(seq);
    auto value = 0LL;
    for (int k = 0; k < lfsr.size(); k++) value = (value + seq[2 * n - 1 - k] * lfsr[k]) % MODULO;
    cout << (value + MODULO) % MODULO;
}
