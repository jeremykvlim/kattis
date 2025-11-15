#include <bits/stdc++.h>
using namespace std;

template <typename T>
vector<complex<T>> quadratic_roots(T a, T b, T c) {
    if (fabs(a) < 1e-8 && fabs(b) < 1e-8) return {};
    if (fabs(a) < 1e-8) return {(-c / b)};

    complex<T> discriminant(b * b - 4 * a * c, 0);
    return {(-b + sqrt(discriminant)) / (2 * a), (-b - sqrt(discriminant)) / (2 * a)};
}

double normal_cdf(double z) {
    return 0.5 * erfc(-z / sqrt(2));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    long double p;
    cin >> n >> p;

    auto buy = floor((long double) (n - 1) / 100);
    if (p == 1) {
        cout << fixed << setprecision(6) << buy * 5;
        exit(0);
    }

    auto q = 1 - p;
    if (n <= 1000) {
        vector<double> dp1(101, 0), dp2(101, 0), a(101), b(101);
        while (n--) {
            a[0] = 1;
            b[0] = 0;
            for (int i = 1; i <= 100; i++) {
                a[i] = q * a[i - 1];
                b[i] = q * b[i - 1] + p * dp2[i - 1];
            }
            dp1[0] = (1 + b[100]) / (1 - a[100]);
            for (int i = 1; i <= 100; i++) dp1[i] = q * dp1[i - 1] + p * dp2[i - 1];
            dp2.swap(dp1);
        }
        cout << fixed << setprecision(6) << dp2[100] * 5;
        exit(0);
    }

    double l = 1, r = 12, m;
    while (l + 1e-6 < r && l + l * 1e-6 < r) {
        m = l + (r - l) / 2;

        if (1 - normal_cdf(m) <= 1e-12) r = m;
        else l = m;
    }

    auto bound = [&](double m) {
        return pow(quadratic_roots(p, m * sqrt(p * q), - ((long double) n - 0.5))[0].real(), 2);
    };
    buy = max(buy, floorl(bound(m) / 100));
    for (auto k = buy + 1; k <= ceill(bound(-m) / 100); k++) {
        auto mu = 100 * k * p, sigma = sqrt(mu * q);
        buy += normal_cdf(((long double) n - 0.5 - mu) / sigma);
    }
    cout << fixed << setprecision(6) << buy * 5;
}
