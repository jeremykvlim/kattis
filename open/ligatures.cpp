#pragma GCC optimize("Ofast,unroll-loops")
#pragma GCC target("avx2")
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q, k;
    string s;
    cin >> n >> q >> k >> s;
    n -= n & 1;

    const int size = 1024, total = 26 * 26 + 1;
    int blocks = (q + size - 1) / size;

    auto encode = [&](int i) {
        return 26 * (s[i] - 'a') + s[i + 1] - 'a';
    };

    vector<int> pairs(n, total - 1);
    for (int i = 0; i < s.size() - 1; i++) pairs[i] = encode(i);

    vector<bitset<size>> suggestions(blocks * total);
    for (int i = 0; i < q; i++) {
        cin >> s;

        auto row = suggestions.begin() + (i / size) * total;
        for (int j = 0; j < k; j++) row[encode(2 * j)][i % size] = true;
    }

    auto full_adder = [](auto &a, auto &b, const auto &c) {
        auto temp = a ^ b;
        b = (a & b) | (c & temp);
        a = temp ^ c;
    };

    for (int b = 0; b < blocks; b++) {
        auto row = suggestions.begin() + b * total;
        bitset<size> carry, picked;
        array<bitset<size>, 19> sums, temps;
        for (auto i = 0U; i < n;) {
            picked = (~picked) & row[pairs[i++]];
            carry = picked;
            auto temp = carry;
            picked = (~picked) & row[pairs[i++]];
            carry = picked;
            full_adder(sums[0], carry, temp);

            int bit = countr_zero(i);
            for (int j = 1; j < bit; j++) full_adder(sums[j], carry, temps[j]);
            if (bit < 19) temps[bit] = carry;
        }

        carry.reset();
        for (int bit = 1; bit < 19; bit++) full_adder(sums[bit], carry, ((n >> bit) & 1) ? temps[bit] : bitset<size>(0));

        for (int _ = 0; _ < min(size, q - b * size); _++) {
            int count = 0;
            for (int bit = 0; bit < 19; bit++) {
                count += sums[bit][0] << bit;
                sums[bit] >>= 1;
            }
            cout << count << "\n";
        }
    }
}
