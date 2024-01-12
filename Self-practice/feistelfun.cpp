#include <bits/stdc++.h>
using namespace std;

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
        r_in ^= ((l_in << 1) ^ (l_in >> (m - 1))) & mask;
        swap(l_in, r_in);
    }

    auto rounds = LLONG_MAX;
    for (auto i = 0LL; i < 1 << upper; i++) {
        if (seen.count((r_out << m) | l_out)) rounds = min(rounds, seen[(r_out << m) | l_out] + (i << upper));
        l_out ^= ((l_in << ((1 << upper) % m)) >> m) ^ (l_in << ((1 << upper) % m)) & mask;
        r_out ^= ((r_in << ((1 << upper) % m)) >> m) ^ (r_in << ((1 << upper) % m)) & mask;
        swap(l_in, l_out);
        swap(r_in, r_out);
    }

    cout << rounds + 1;
}
