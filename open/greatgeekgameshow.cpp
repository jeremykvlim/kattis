#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    vector<double> ring(k + 1, 0);
    double win = 0, sum = ring[0] = 1;
    for (int i = 1; i <= n; i++) {
        ring[i % (k + 1)] = win = sum / i;
        sum += win;
        if (i >= k) sum -= ring[(i - k) % (k + 1)];
    }
    cout << fixed << setprecision(6) << win;
}
