#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;

    vector<pair<long long, int>> pfs;
    for (int p = 2; p <= sqrt(n); p == 2 ? p++ : p += 2) {
        int pow = 0;
        while (!(n % p)) {
            n /= p;
            pow++;
        }
        if (pow) pfs.emplace_back(p, pow);
    }
    if (n > 1) pfs.emplace_back(n, 1);

    auto sum = 1LL;
    for (auto [pf, pow] : pfs) {
        auto s = 1LL;
        for (int i = 0; i < pow; i++) s = pf * s + 2 * i + 3;
        sum *= s;
    }

    cout << sum;
}
