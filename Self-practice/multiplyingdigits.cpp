#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int b;
    long long N;
    cin >> b >> N;

    if (N == 1) {
        cout << "1";
        exit(0);
    }

    vector<int> factors;
    auto num = N;
    for (int i = 2; i < b; i++)
        if (!(N % i)) {
            factors.emplace_back(i);
            while (!(num % i)) num /= i;
        }

    if (num > 1) {
        cout << "impossible";
        exit(0);
    }

    map<long long, long long> f_inv{{1, 0}};
    for (auto [n, x] : f_inv)
        for (int i = lower_bound(factors.begin(), factors.end(), x % b) - factors.begin(); i < factors.size(); i++) {
            int f = factors[i];
            if ((__int128) x * b + f > LLONG_MAX || N < n * f) break;
            if (!(N % (n * f))) f_inv[n * f] = min(f_inv[n * f] ? f_inv[n * f] : LLONG_MAX, x * b + f);
        }

    cout << f_inv[N];
}
