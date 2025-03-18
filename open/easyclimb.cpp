#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, d, h1;
        cin >> n >> d >> h1;

        vector<long long> h(2, h1);
        auto update = [&](int m) {
            for (int i = 0; i < m; i++) h[i] -= d;
            for (int i = m; i < 2 * m; i++) h[i] += d;
        };

        auto sum = 0LL;
        for (int i = 0; i < n - 2; i++) {
            long long hi;
            cin >> hi;

            update(i + 1);

            if (hi < h[0]) {
                sum += h[0] - hi;
                hi = h[0];
            }

            if (hi > h.back()) {
                sum += hi - h.back();
                hi = h.back();
            }

            sum += abs(hi - h[0]);
            h.emplace_back(hi);
            h.emplace_back(hi);
            sort(h.begin(), h.end());
        }
        update(n - 1);

        int hn_minus_1;
        cin >> hn_minus_1;

        if (hn_minus_1 < h[0] || h.back() < hn_minus_1) {
            cout << "impossible\n";
            continue;
        }

        for (int i = 0, count = 2 - n; i < 2 * n - 3; i++) {
            if (hn_minus_1 <= h[i + 1]) {
                cout << sum + (hn_minus_1 - h[i]) * count << "\n";
                break;
            }
            sum += (h[i + 1] - h[i]) * count++;
        }
    }
}
