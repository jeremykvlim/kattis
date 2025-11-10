#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n, p;
    cin >> n >> p;

    auto product = 1LL;
    for (auto v = n; v; v /= p) product *= v % p + 1;
    cout << n + 1 - product;
}
