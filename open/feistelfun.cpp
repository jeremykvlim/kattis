#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m;
    cin >> m;

    int bound = m / 2 + 1;
    unordered_map<bitset<80>, long long> seen;
    bitset<80> x1(0), y1(1), x2(1), y2(0);

    auto f = [&](bitset<80> y, int shift = 1) -> bitset<80> {
        return ((y << shift) >> m) ^ (y << shift) & bitset<80>((1LL << m) - 1);
    };

    for (auto i = 0LL; i < 1 << bound; i++) {
        if (!seen.count((y1 << m) | x1)) seen[(y1 << m) | x1] = i;
        x1 ^= f(y1);
        swap(y1, x1);
    }

    auto rounds = LLONG_MAX;
    for (auto i = 0LL; i < 1 << bound; i++) {
        if (seen.count((y2 << m) | x2)) rounds = min(rounds, seen[(y2 << m) | x2] + (i << bound));
        x2 ^= f(y1, (1 << bound) % m);
        y2 ^= f(x1, (1 << bound) % m);
        swap(y1, x2);
        swap(y2, x1);
    }

    cout << rounds + 1;
}
