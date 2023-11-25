#include <bits/stdc++.h>
using namespace std;

long long f(long long n, long long b, int size, vector<int> &factors, unordered_set<int> &prime_factors) {
    if (n < b) return n;
    auto x = LLONG_MAX;
    for (int i = size, depth = 0; i >= 0 && factors[i] * factors[i] >= b; i--) {
        if (n % factors[i] ||
            any_of(prime_factors.begin(), prime_factors.end(), [&](int p){return factors[i] * p < b && !(n % (factors[i] * p));})) continue;
        auto next = f(n / factors[i], b, i, factors, prime_factors) * b + factors[i];
        if (next <= 0) continue;
        x = min(x, next);
        if (depth++ == 3) break;
    }
    return x;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long b, n;
    cin >> b >> n;

    if (n == 1) {
        cout << "1\n";
        exit(0);
    }

    vector<int> factors;
    for (int i = 2; i < b; i++) if (!(n % i)) factors.emplace_back(i);

    unordered_set<int> prime_factors;
    auto quotient = n;
    for (int &factor: factors)
        while (!(quotient % factor)) {
            quotient /= factor;
            prime_factors.insert(factor);
        }

    if (quotient > 1) {
        cout << "impossible";
        exit(0);
    }

    cout << f(n, b, factors.size() - 1, factors, prime_factors);
}
