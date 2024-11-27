#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    for (int x = 1; x <= t; x++) {
        int r, k, n;
        cin >> r >> k >> n;

        vector<int> g(n);
        for (int &gi : g) cin >> gi;

        int i = 0, j = 1;
        auto y = 0LL;
        auto add = [&]() {
            int sum = 0;
            for (int _ = 0; _ < n && sum + g[i] <= k; _++) {
                sum += g[i];
                i = (i + 1) % n;
            }
            y += sum;
        };

        vector<int> ride(n, 0);
        vector<long long> earnings(n, 0);
        for (; j <= r && !ride[i]; j++) {
            ride[i] = j;
            earnings[i] = y;
            add();
        }

        if (ride[i]) {
            int len = j - ride[i], remaining = r - j + 1;
            y += (y - earnings[i]) * (remaining / len);

            remaining %= len;
            while (remaining--) add();
        }

        cout << "Case #" << x << ": " << y << "\n";
    }
}
