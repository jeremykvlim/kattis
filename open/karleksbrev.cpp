#include <bits/stdc++.h>
using namespace std;

string encode(string S) {
    auto h = 0LL;
    for (int i = 0; i < 100; i++)
        if (S[i] == '1') h ^= (i + 1) * (long long) 1e9;

    for (int i = 0; i < 50; i++) S += '0' + ((h >> i) & 1);
    return S;
}

string decode(string M) {
    auto h1 = 0LL;
    for (int i = 0; i < 150; i++)
        if (M[i] == '1') h1 ^= i < 100 ? (i + 1) * (long long) 1e9 : 1LL << (i - 100);

    for (int l = 0; l < 150; l++) {
        auto h2 = 0LL;
        for (int r = l; r < 150; r++) {
            h2 ^= r < 100 ? (r + 1) * (long long) 1e9 : 1LL << (r - 100);
            if (h1 == h2) {
                for (int i = l; i <= r && i < 100; i++) M[i] ^= 1;
                return M.substr(0, 100);
            }
        }
    }
}