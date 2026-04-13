#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, long long>> solutions;
    for (int x = 1; x <= 2 * n; x++) {
        int g = gcd(n, x), a = n / g, b = x / g;
        if (a != b)
            for (int k = (x + a - 1) / a; k <= x / abs(a - b); k++) {
                auto y = (long long) a * k;
                if ((x ^ y) == (long long) b * k) solutions.emplace_back(x, y);
            }
    }

    cout << solutions.size() << "\n";
    for (auto [x, y] : solutions) cout << x << " " << y << "\n";
}