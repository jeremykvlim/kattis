#include <bits/stdc++.h>
using namespace std;

int factors(vector<int> exponents, vector<int> primes, int f) {
    int total = accumulate(exponents.begin(), exponents.end(), 0);

    if (primes.empty()) primes.emplace_back(0);
    else 
        for (int i = 0; ++primes[i] >= exponents.size(); i++) 
            if (i == primes.size() - 1) {
                primes = vector<int>(primes.size() + 1, 0);
                break;
            } else {
                primes[i] = primes[i + 1] + 1;
                fill(primes.begin(), primes.begin() + i, primes[i]);
            }

    if (f >= total / primes.size()) return f;

    vector<int> reduced = exponents;
    for (int p : primes)
        if (!reduced[p]--) {
            reduced.clear();
            break;
        }

    if (!reduced.empty()) f = factors(reduced, primes, max(f - 1, 0)) + 1;
    return factors(exponents, primes, f);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long x;
    cin >> x;
    
    vector<int> exponents, primes;
    int points = 0;
    for (long long b = 2; b * b <= x; b++) {
        int power = 0;
        while (!(x % b)) {
            x /= b;
            power++;
        }
        if (power) {
            points++;
            if (power > 1) exponents.emplace_back(power - 1);
        }
    }
    
    if (x > 1) points++;
    primes.emplace_back(exponents.size());
    if (!exponents.empty()) points += factors(exponents, primes, 0);
    cout << points;
}
