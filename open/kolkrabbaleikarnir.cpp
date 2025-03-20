#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    double sum = 0;
    for (int k = 0; k <= min(n, m); k++) {
        auto lg_nck = lgamma(n + 1) - lgamma(k + 1) - lgamma(n - k + 1);
        sum += exp(lg_nck - n * log(2)) * (m - k);
    }
    cout << fixed << setprecision(6) << sum;
}
