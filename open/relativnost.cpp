#include <bits/stdc++.h>
using namespace std;

const int MODULO = 1e4 + 7;

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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, c;
    cin >> n >> c;

    vector<int> a(n + 1), b(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];
    for (int i = 1; i <= n; i++) cin >> b[i];

    int total = 1, rgb = 1, bw = 1, count = 0;
    for (int i = 1; i <= n; i++) {
        a[i] %= MODULO;
        b[i] %= MODULO;

        if (!b[i]) {
            count++;
            rgb = mul(rgb, a[i], MODULO);
            continue;
        }

        a[i] = mul(a[i], pow(b[i], MODULO - 2, MODULO), MODULO);
        total = mul(total, a[i] + 1, MODULO);
        bw = mul(bw, b[i], MODULO);
    }

    vector<int> dp(c);
    dp[0] = 1;
    for (int i = 1; i <= n; i++) {
        if (!b[i]) continue;
        for (int j = c - 1; j; j--) dp[j] = (dp[j] + mul(dp[j - 1], a[i], MODULO)) % MODULO;
    }

    int q;
    cin >> q;

    while (q--) {
        int p, ap, bp;
        cin >> p >> bp >> ap;

        ap %= MODULO;
        bp %= MODULO;

        if (b[p]) {
            bw = mul(bw, pow(b[p], MODULO - 2, MODULO), MODULO);
            total = mul(total, pow(a[p] + 1, MODULO - 2, MODULO), MODULO);
            for (int j = 1; j < c; j++) dp[j] = (dp[j] - mul(dp[j - 1], a[p], MODULO)) % MODULO;
        } else {
            count--;
            rgb = mul(rgb, pow(a[p], MODULO - 2, MODULO), MODULO);
        }

        a[p] = bp;
        b[p] = ap;

        if (b[p]) {
            a[p] = bp = mul(bp, pow(ap, MODULO -2, MODULO), MODULO);
            bw = mul(bw, b[p], MODULO);
            total = mul(total, a[p] + 1, MODULO);
            for (int j = c - 1; j; j--) dp[j] = (dp[j] + mul(dp[j - 1], a[p], MODULO)) % MODULO;
        } else {
            count++;
            rgb = mul(rgb, a[p], MODULO);
        }

        if (count > c) {
            cout << "0\n";
            continue;
        }

        int purchases = total;
        for (int j = 0; j < c - count; j++) purchases = (purchases - dp[j]) % MODULO;
        cout << mul(purchases, mul(bw, rgb, MODULO), MODULO) << "\n";
    }
}
