#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n, m, k;
    cin >> n >> m >> k;

    long long damage = 0;
    for (int i = n % k; i < min(n + 1, m * k); i += k) damage = max(damage, (((n - i) / k + 1) * (n + i) - 2 * i + (i / m) * (m*m + m) + (i % m) * (i % m + 1)) / 2);
    
    cout << damage;
}
