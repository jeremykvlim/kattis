#include <bits/stdc++.h>
using namespace std;

bitset<80> f(bitset<80> l, bitset<80> r, bitset<80> mask, long long shift, int m) {
    r ^= ((l << shift) >> m) ^ (l << shift) & mask;
    return r;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m;
    cin >> m;

    int upper = m / 2 + 1;
    unordered_map<bitset<80>, long long> seen;
    bitset<80> l_in(1), r_in(0), l_out(1), r_out(0), mask((1LL << m) - 1);

    for (auto i = 0LL; i < 1 << upper; i++) {
        if (!seen.count((l_in << m) | r_in)) seen[(l_in << m) | r_in] = i;
        r_in = f(l_in, r_in, mask, 1, m);
        swap(l_in, r_in);
    }

    auto rounds = LLONG_MAX;
    for (auto i = 0LL; i < 1 << upper; i++) {
        if (seen.count((r_out << m) | l_out)) rounds = min(rounds, seen[(r_out << m) | l_out] + (i << upper));
        l_out = f(l_in, l_out, mask, ((1 << upper) % m), m);
        r_out = f(r_in, r_out, mask, ((1 << upper) % m), m);
        swap(l_in, l_out);
        swap(r_in, r_out);
    }

    cout << rounds + 1;
}
