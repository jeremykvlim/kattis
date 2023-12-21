#include<bits/stdc++.h>
using namespace std;

constexpr int MODULO = 1e9 + 7;

long long f(long long x, unordered_map<long long, long long> &dp) {
    if (!dp.count(x)) {
        auto even = x / 2, odd = (x + 1) / 2;
        dp[x] = (even + 2 * (odd - 1) + f(odd, dp) + f(even, dp)) % MODULO;
    }

    return dp[x];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long l, r;
    cin >> l >> r;

    unordered_map<long long, long long> dp{{0, 0}, {1, 0}};
    cout << (f(r, dp) - f(l - 1, dp) + MODULO) % MODULO;
}
