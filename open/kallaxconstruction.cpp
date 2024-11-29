#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int b, k;
    cin >> b >> k;

    vector<int> dp1(2e3 + 1), dp2(2e3 + 1);
    iota(dp1.begin(), dp1.end(), 0);
    int smallest = INT_MAX;
    while (k--) {
        int l;
        cin >> l;

        fill(dp2.begin(), dp2.end(), 1e9);
        dp2[0] = 0;
        while (l--) {
            int n;
            cin >> n;

            int bolts = -1;
            for (int i = n; i <= 2e3; i++)
                if (dp1[i] < 1e9) {
                    bolts = dp1[i];
                    break;
                }

            for (int i = n; i <= 2e3; i++)
                if (dp2[i - n] < 1e9) dp2[i] = min(dp2[i], dp2[i - n] + bolts);

            if (b <= bolts) smallest = min(smallest, n);
        }
        swap(dp1, dp2);
    }
    cout << (smallest == INT_MAX ? "impossible" : to_string(smallest));
}
