#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, x = 1;
    while (cin >> n && n) {
        cout << "Output for data set " << x++ << ", " << n << " bytes:\n";

        int t = 0;
        for (int total = 0, interval = 0; total < n;) {
            int b;
            cin >> b;

            total += b;
            interval += b;
            if (!(++t % 5)) {
                if (!interval) cout << "   Time remaining: stalled\n";
                else cout << "   Time remaining: " << ceil((n - total) / ((double) interval / 5)) << " seconds\n";
                interval = 0;
            }
        }
        cout << "Total time: " << t << " seconds\n\n";
    }
}
