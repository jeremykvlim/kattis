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

    vector<long long> pf;
    auto temp = n;
    for (auto i = 2LL; i <= sqrt(n); i == 2 ? i++ : i += 2)
        if (!(n % i)) {
            pf.emplace_back(i);
            while (!(n % i)) n /= i;
            temp /= i;
        }

    if (n > 1) {
        pf.emplace_back(n);
        n = temp / n;
    } else n = temp;

    auto div = divisors(n);
    sort(div.begin(), div.end());
    unordered_set<long long> s(div.begin() + 1, div.end());
    for (auto &f : pf) s.erase(f);

    vector<int> dp(div.size(), INT_MIN);
    dp[0] = 0;
    for (auto d : s)
        for (int i = div.size() - 1, j = div.size(); ~i; i--) {
            while (j && d * div[j - 1] > div[i]) j--;
            if (!j) break;

            if (d * div[j - 1] == div[i]) dp[i] = max(dp[i], dp[j - 1] + 1);
        }

    cout << *max_element(dp.begin(), dp.end()) + pf.size();
}
