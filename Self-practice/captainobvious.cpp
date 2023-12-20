#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int k, m;
        cin >> k >> m;

        vector<long long> spot(k), coefficients(k + 1, 0);
        for (auto &p : spot) cin >> p;

        coefficients[0] = 1;
        for (int i = 0, curr = 1, prev = 0; i < k; i++) {
            int next = (curr + prev) % m;
            prev = curr;
            curr = next;

            rotate(coefficients.rbegin(), coefficients.rbegin() + 1, coefficients.rend());
            for (int j = 0; j < k; j++) coefficients[j] = (coefficients[j] - coefficients[j + 1] * curr) % m;
        }

        long long p = 0;
        for (int i = 0; i < k; i++) p = (p + (spot[i] * coefficients[i]) % m) % m;
        cout << (m - p) % m << "\n";
    }
}
