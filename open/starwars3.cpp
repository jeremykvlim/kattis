#include <bits/stdc++.h>
using namespace std;

template <typename T>
T kitamasa(const vector<T> &c, const vector<T> &a, long long k) {
    int n = a.size();

    auto mul = [&](const vector<T> &x, const vector<T> &y) {
        vector<T> z(2 * n + 1);
        for (int i = 0; i <= n; i++)
            for (int j = 0; j <= n; j++) z[i + j] += x[i] * y[j];

        for (int i = 2 * n; i > n; i--)
            for (int j = 0; j < n; j++) z[i - j - 1] += z[i] * c[j];

        z.resize(n + 1);
        return z;
    };

    vector<T> base(n + 1, 0);
    base[1] = 1;
    auto pow = [&](vector<T> base, long long exponent) {
        vector<T> value(n + 1);
        value[0] = 1;
        while (exponent) {
            if (exponent & 1) value = mul(value, base);
            base = mul(base, base);
            exponent >>= 1;
        }
        return value;
    };
    auto value = pow(base, k + 1);

    T kth = 0;
    for (int i = 0; i < n; i++) kth += value[i + 1] * a[i];
    return kth;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long double p;
    cin >> n >> p;

    if (p == 1) {
        cout << fixed << setprecision(6) << 5 * floor((long double) (n - 1) / 100);
        exit(0);
    }

    int m = 100;
    auto q = 1 - p;
    vector<long double> p_exact(m + 1);
    p_exact[0] = powl(q, m);
    for (int k = 1; k <= m; k++) p_exact[k] = p_exact[k - 1] * (m - k + 1) / k * p / q;
    auto d = 1 - p_exact[0];

    vector<long double> dp(m + 1, 0);
    for (int i = 1; i <= m; i++) {
        dp[i] = p_exact[0];
        for (int k = 1; k < i; k++) dp[i] += p_exact[k] * (1 + dp[i - k]);
        dp[i] /= d;
    }

    if (n <= m) {
        cout << fixed << setprecision(6) << 5 * dp[n];
        exit(0);
    }

    vector<long double> c(m), a(m);
    for (int i = 1; i <= m; i++) {
        c[i - 1] = p_exact[i] / d;
        a[i - 1] = dp[i] - i / (m * p);
    }
    cout << fixed << setprecision(6) << 5 * (kitamasa(c, a, n - 1) + n / (m * p));
}
