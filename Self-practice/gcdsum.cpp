#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;

    unordered_map<long long, int> pfs;
    for (int p = 2; p <= sqrt(n); p == 2 ? p++ : p += 2)
        while (!(n % p)) {
            n /= p;
            pfs[p]++;
        }
    if (n > 1) pfs[n]++;

    auto sum = 1LL;
    for (auto [pf, pow] : pfs) {
        auto s = 1LL;
        for (int i = 0; i < pow; i++) s = pf * s + 2 * i + 3;
        sum *= s;
    }

    cout << sum;
}
