#include <bits/stdc++.h>
using namespace std;

pair<vector<int>, vector<bool>> sieve(int n) {
    vector<int> spf(n + 1, 0), primes;
    vector<bool> prime(n + 1, false);
    for (int i = 2; i <= n; i++) {
        if (!spf[i]) {
            spf[i] = i;
            primes.emplace_back(i);
            prime[i] = true;
        }

        for (int p : primes) {
            auto j = (long long) i * p;
            if (j > n) break;
            spf[j] = p;
            if (p == spf[i]) break;
        }
    }
    return {spf, prime};
}

vector<vector<int>> generate_prime_factors(int n, const vector<int> &spf) {
    vector<vector<int>> pfs(n + 1);
    for (int i = 2; i <= n; i++)
        for (int j = i; j > 1;) {
            int p = spf[j];
            for (; !(j % p); j /= p);
            pfs[i].emplace_back(p);
        }
    return pfs;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto [spf, prime] = sieve(1e4 + 7);
    auto pfs = generate_prime_factors(1e4 + 7, spf);
    vector<vector<int>> dp(1e4 + 8, vector<int>(3, 1e9));
    for (int p = 2, prev = 1; p <= 1e4 + 7; p == 2 ? p++ : p += 2) {
        if (!prime[p]) continue;
        dp[p][0] = 1;
        for (int i = p - 1; i > prev; i--) {
            int j = i + 1;
            dp[i][0] = dp[j][2];
            for (int pf : pfs[i])
                if (dp[i][0] >= min(i / pf, dp[i / pf][2])) {
                    dp[i][0] = min(i / pf, dp[i / pf][2]);
                    j = i / pf;
                }
            dp[i][1] = dp[j][0];
            dp[i][2] = dp[j][1];
        }
        prev = p;
    }

    int n;
    cin >> n;

    unordered_map<char, int> indices{{'O', 0}, {'E', 1}, {'I', 2}};
    vector<int> scores(3, 0);
    while (n--) {
        char c;
        int i;
        cin >> c >> i;

        for (int j = 0; j < 3; j++) scores[(indices[c] + j) % 3] += min(i, dp[i][j]);
    }

    for (int s : scores) cout << s << " ";
}
