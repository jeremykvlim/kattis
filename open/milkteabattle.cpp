#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, x;
    cin >> n >> k >> x;

    if (n == k) {
        cout << "FINAL 1 1 " << x << flush;
        exit(0);
    }

    vector<double> v(n + 1, 0);
    vector<int> c;
    double V;
    auto pour = [&]() -> void {
        int p = c.size();
        cout << "POUR " << p << " ";
        for (int ci : c) {
            cout << ci << " " << fixed << setprecision(6) << V - v[ci] << " ";

            v[ci] = V;
        }
        cout << "\n" << flush;

        int i;
        cin >> i;

        for (int j = 0; j < k; j++) v[i + j] = 0;
    };

    int limit = (n + k - 1) / k;
    for (int i = 0; i <= 2000 - limit; i++) {
        c.clear();

        V = x;
        for (int j = 1; j <= limit * k; j += k) {
            V += v[j];
            c.emplace_back(j);
        }
        V /= limit;
        pour();
    }

    for (int i = 0; i < limit - 2; i++) {
        c.clear();
        
        double most = 0;
        for (int j = 1; j <= limit * k; j += k) most = max(most, v[j]);

        V = x;
        for (int j = 1; j <= limit * k && c.size() < limit - i - 1; j += k) {
            if (v[j] + 0.9 < most) continue;
            V += v[j];
            c.emplace_back(j);
        }
        V /= limit - i - 1;
        pour();
    }

    cout << "FINAL 1 " << max_element(v.begin(), v.end()) - v.begin() << " " << x << flush;
}
