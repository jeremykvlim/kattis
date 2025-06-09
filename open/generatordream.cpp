#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int p;
    string bs;
    cin >> p >> bs;

    if (p == 2) {
        cout << bs;
        exit(0);
    }

    auto v = 0LL;
    for (char c : bs.substr(1)) {
        v <<= 1;
        v += c == '1';
    }

    auto p2 = 1LL << (bs.size() - 1), x = (v * p + p2 - 1) / p2;
    cout << x + ((x & 1) != bs[0] - '0');
}
