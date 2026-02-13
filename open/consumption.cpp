#include <bits/stdc++.h>
using namespace std;

template <typename T>
T kitamasa(const vector<T> &c, const vector<T> &a, long long k) {
    int n = a.size();

    auto mul = [&](const vector<T> &x, const vector<T> &y) {
        vector<T> z(2 * n + 1, 0);
        for (int i = 0; i <= n; i++)
            if (x[i])
                for (int j = 0; j <= n; j++) z[i + j] += x[i] * y[j];

        for (int i = 2 * n; i > n; i--)
            if (z[i])
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

    vector<vector<int>> C(6, vector<int>(6));
    for (int n = 0; n <= 5; n++) {
        C[n][0] = C[n][n] = 1;
        for (int k = 1; k < n; k++) C[n][k] = C[n - 1][k - 1] + C[n - 1][k];
    }

    for (int n, i = 1; cin >> n && n;) {
        cout << "Set " << i++ << "\n";

        int g = 0;
        vector<int> w(n);
        for (int &wi : w) {
            cin >> wi;

            g = gcd(g, wi);
        }
        sort(w.begin(), w.end());

        int mod = 1;
        for (int &wi : w) mod = lcm(mod, wi /= g);

        int m = mod * n;
        vector<long long> dp(m, 0);
        dp[0] = 1;
        for (int wi : w)
            for (int j = 0; j + wi < m; j++) dp[j + wi] += dp[j];

        vector<long long> c(n), a(n);
        for (int j = 0; j < n; j++) c[j] = (j & 1 ? -1 : 1) * C[n][j + 1];

        auto boxes = [&](long long sum) {
            if (sum < m) return dp[sum];

            auto k = sum / mod;
            int r = sum % mod;

            for (int j = 0; j < n; j++) a[j] = dp[r + j * mod];
            return kitamasa(c, a, k);
        };

        int q;
        cin >> q;

        int w0 = w[0];
        while (q--) {
            long long p;
            cin >> p;

            auto W = (100 * p + 1) / g;
            for (int rem = 0; rem < w0; rem++) {
                auto weight = [&](long long quo) -> long long {
                    return w0 * quo + rem;
                };

                long long l = 0, r = 2, mid;
                for (; boxes(weight(r)) < p; l = r, r <<= 1)
                    if (weight(l + 1) >= W) goto next;

                while (l + 1 < r) {
                    mid = (l + r) / 2;

                    if (boxes(weight(mid)) >= p) r = mid;
                    else l = mid;
                }
                W = min(W, weight(r));
                next:;
            }
            if (W == (100 * p + 1) / g) cout << "no candy for you\n";
            else cout << W * g << "\n";
        }
    }
}
