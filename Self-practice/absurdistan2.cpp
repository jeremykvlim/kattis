#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;

    vector<double> probability(n + 1, 0), binomial(n + 1, 1);
    for (int i = 1; i <= n; i++) {
        probability[i] = pow(i - 1, i);
        for (int j = 1; j < i; j++) {
            binomial[j] = binomial[j - 1] * (i - j) / j;
            if (j > 1) probability[i] -= pow(i - j - 1, i - j) * binomial[j - 1] * probability[j];
        }
    }

    cout << fixed << setprecision(8) << probability[n] / pow(n - 1, n);
}
