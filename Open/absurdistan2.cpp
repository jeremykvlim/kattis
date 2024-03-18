#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<double> prob(n + 1, 0), C(n + 1, 1);
    for (int i = 1; i <= n; i++) {
        prob[i] = pow(i - 1, i);
        for (int j = 1; j < i; j++) {
            C[j] = C[j - 1] * (i - j) / j;
            if (j > 1) prob[i] -= pow(i - j - 1, i - j) * C[j - 1] * prob[j];
        }
    }

    cout << fixed << setprecision(8) << prob[n] / pow(n - 1, n);
}
