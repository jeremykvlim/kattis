#include <bits/stdc++.h>
using namespace std;

double C(int n, int k) {
    return !k ? 1 : C(n - 1, k - 1) * n / k;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> x(n + 1);
    for (int i = n; ~i; i--) cin >> x[i];

    cin >> n;

    vector<int> y(n + 1);
    for (int i = n; ~i; i--) cin >> y[i];

    int degree = max(x.size(), y.size()) - 1, a = degree & 1 ? x[degree] : y[degree], b = degree & 1 ? y[degree] : -x[degree];
    if (!a && !b) {
        cout << "1 0 0";
        exit(0);
    }

    vector<double> odd(degree + 1), even(degree + 1);
    for (int i = 0; i <= degree; i++) {
        odd[i] = a * x[i] + b * y[i];
        even[i] = b * x[i] - a * y[i];
    }

    double t = !degree ? 0 : degree & 1 ? -odd[degree - 1] / (degree * odd[degree]) : -even[degree - 1] / (degree * even[degree]);
    for (int i = degree; i; i--) {
        auto &curr = i & 1 ? odd : even, &prev = i & 1 ? even : odd;

        if (abs(prev[i]) > 1e-6) {
            cout << "0 0 0";
            exit(0);
        }

        auto arg = 1.0;
        for (int j = i - 1; ~j; j--)  {
            arg *= -t;
            curr[j] -= C(i, j) * arg * curr[i];
        }
    }

    cout << fixed << setprecision(6) << a << " " << b << " " << -odd[0];
}
