#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto bit_reverse = [](unsigned long long x) {
        x = (x & 0xAAAAAAAAAAAAAAAAULL) >> 1 | (x & 0x5555555555555555ULL) << 1;
        x = (x & 0xCCCCCCCCCCCCCCCCULL) >> 2 | (x & 0x3333333333333333ULL) << 2;
        x = (x & 0xF0F0F0F0F0F0F0F0ULL) >> 4 | (x & 0x0F0F0F0F0F0F0F0FULL) << 4;
        return byteswap(x);
    };

    auto c = 12157665459056928801ULL;
    for (int _ = 0; _ < 10; _++) {
        int x;
        cin >> x;

        auto place_bits = [](unsigned mask, int b) {
            auto state = 0ULL;
            for (int i = 0; i < 32; i++) state |= (unsigned long long) ((mask >> i) & 1) << (2 * i + b);
            return state;
        };

        auto build = [&](int b) {
            int x_half = 0;
            for (int i = 0; i < 14; i++) x_half |= ((x >> (2 * i + b)) & 1) << i;

            unordered_map<unsigned, int> memo;
            for (int k_half = 0; k_half < 1 << 18; k_half++) {
                auto mask = (unsigned) (x_half << 18) | k_half;
                for (int i = 0; i < 16; i++) mask = (mask << 16) | ((~(mask >> 15) | ((mask >> 16) & (mask >> 11))) & USHRT_MAX);
                memo[mask] = k_half;
            }

            vector<pair<unsigned long long, int>> half;
            for (auto [mask, k_half] : memo) half.emplace_back(place_bits(mask, b), k_half);
            return half;
        };
        auto even = build(0), odd = build(1);

        auto hash = ~0ULL, k = 0ULL;
        for (auto [h_even, k_half_even] : even)
            for (auto [h_odd, k_half_odd] : odd) {
                unsigned __int128 h = h_even | h_odd;
                for (int _ = 0; _ < 16; _++) {
                    h += bit_reverse(h);
                    h ^= c;
                    if (h > ULLONG_MAX) h >>= 1;
                }

                if (hash > h) {
                    hash = h;
                    k = place_bits(k_half_even, 0) | (place_bits(k_half_odd, 1));
                }
            }
        cout << k << "\n";
    }
}
