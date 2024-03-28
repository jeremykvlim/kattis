#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e9 + 7;

vector<int> sieve(int n) {
    vector<int> factors(n + 1), primes{2};
    iota(factors.begin(), factors.end(), 0);
    for (int p = 3; p <= n; p += 2)
        if (factors[p] == p) {
            primes.emplace_back(p);
            for (auto i = (long long) p * p; i <= n; i += 2 * p)
                if (factors[i] == i) factors[i] = p;
        }

    return primes;
}

long long mul(long long x, long long y, long long mod) {
    auto product = x * y - mod * (long long) (1.L / mod * x * y);
    return product + mod * (product < 0) - mod * (product >= mod);
}

long long pow(long long base, long long exponent, long long mod) {
    auto value = 1LL;
    for (; exponent; exponent >>= 1) {
        if (exponent & 1) value = mul(value, base, mod);
        base = mul(base, base, mod);
    }

    return value;
}

void divisors(vector<pair<int, int>> &pfs, vector<pair<double, long long>> &divs, pair<double, long long> d = {0, 1}, int i = 0) {
    if (i == pfs.size()) return;

    divisors(pfs, divs, d, i + 1);

    auto [pf, pow] = pfs[i];
    pair<double, long long> p{log(pf), pf};
    while (pow--) {
        d = {d.first + p.first, mul(d.second, p.second, MODULO)};
        divs.emplace_back(d);
        divisors(pfs, divs, d, i + 1);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto primes = sieve(99);
    vector<int> indices(primes.back() + 1), factors(primes.back() + 1, 0);
    for (int i = 0; i < primes.size(); i++) indices[primes[i]] = i;

    string s;
    cin >> s;

    pair<double, long long> K{0, 1};
    auto &[logk, kmod] = K;
    for (int i = 0; i < s.size() / 2; i++) {
        int pf = stoi(s.substr(2 * i, 2));
        logk += log(pf);
        kmod = mul(kmod, pf, MODULO);
        factors[indices[pf]]++;
    }

    vector<pair<int, int>> pfs1, pfs2;
    auto s1 = 1LL, s2 = 1LL;
    for (int i = 0; i < factors.size(); i++) {
        if (!factors[i]) continue;

        (s1 <= s2 ? pfs1 : pfs2).emplace_back(primes[i], factors[i]);
        (s1 <= s2 ? s1 : s2) *= factors[i] + 1;
    }

    vector<pair<double, long long>> divs1{{0, 1}}, divs2{{0, 1}};
    divisors(pfs1, divs1);
    divisors(pfs2, divs2);
    sort(divs1.begin(), divs1.end());
    sort(divs2.rbegin(), divs2.rend());

    auto diff = DBL_MAX;
    long long d;
    for (int i = 0, j = 0; i < divs1.size(); i++) {
        while (divs1[i].first + divs2[j].first > 0.5 * logk && j < divs2.size() - 1) j++;

        for (int k = j - (j > 0); k <= j; k++)
            if (diff > abs(divs1[i].first + divs2[k].first - 0.5 * logk)) {
                diff = abs(divs1[i].first + divs2[k].first - 0.5 * logk);
                d = mul(divs1[i].second, divs2[k].second, MODULO);
            }
    }

    cout << (d + mul(kmod, pow(d, MODULO - 2, MODULO), MODULO)) % MODULO;
}
