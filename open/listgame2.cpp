#include <bits/stdc++.h>
using namespace std;

vector<long long> divisors(long long n) {
    vector<long long> d;
    for (auto i = 1LL; i <= sqrt(n); i++)
        if (!(n % i)) {
            d.emplace_back(i);
            if (n / i != i) d.emplace_back(n / i);
        }

    return d;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    cin >> n;

    vector<long long> pfs;
    auto temp = n;
    for (int p = 2; p <= sqrt(n); p == 2 ? p++ : p += 2)
        if (!(n % p)) {
            pfs.emplace_back(p);
            while (!(n % p)) n /= p;
            temp /= p;
        }

    if (n > 1) {
        pfs.emplace_back(n);
        n = temp / n;
    } else n = temp;

    auto divs = divisors(n);
    sort(divs.begin(), divs.end());
    unordered_set<long long> s(divs.begin() + 1, divs.end());
    for (auto &pf : pfs) s.erase(pf);

    vector<int> dp(divs.size(), INT_MIN);
    dp[0] = 0;
    for (auto d : s)
        for (int i = divs.size() - 1, j = divs.size(); ~i; i--) {
            while (j && d * divs[j - 1] > divs[i]) j--;
            if (!j) break;

            if (d * divs[j - 1] == divs[i]) dp[i] = max(dp[i], dp[j - 1] + 1);
        }

    cout << *max_element(dp.begin(), dp.end()) + pfs.size();
}
