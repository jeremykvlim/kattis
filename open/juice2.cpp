#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    for (int x = 1; x <= t; x++) {
        int n;
        cin >> n;

        vector<array<int, 3>> juice(n);
        for (auto &[a, b, c] : juice) cin >> a >> b >> c;
        sort(juice.begin(), juice.end());

        int y = 0;
        for (int i = 0, j = 1; i < n; i++, j++) {
            int a = juice[i][0], b_and_c = 1e4 - a;
            while (j < n && juice[j][0] == a) {
                i++;
                j++;
            }

            vector<int> count_b(1e4 + 1, 0), count_c(1e4 + 1, 0);
            for (int k = 0; k <= i; k++)
                if (juice[k][1] + juice[k][2] <= b_and_c) {
                    count_b[juice[k][1]]++;
                    count_c[juice[k][2]]++;
                }

            int sum = 0;
            for (int b = 0; b <= b_and_c; b++) {
                sum += count_b[b] - count_c[b_and_c - b + 1];
                y = max(y, sum);
            }
        }

        cout << "Case #" << x << ": " << y << "\n";
    }
}
