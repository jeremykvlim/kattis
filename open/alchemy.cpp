#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<array<int, 6>> circles(n);
    for (int i = 0; i < n; i++) {
        cin >> circles[i][0] >> circles[i][1] >> circles[i][2] >> circles[i][3] >> circles[i][4];

        circles[i][5] = i + 1;
    }
    sort(circles.begin(), circles.end(), [](auto a1, auto a2) {return a1[2] < a2[2];});

    vector<int> energy(n, 0), dp(n, 0), count(n, 0), parent(n, 0), required(n, 0);
    int most = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++)
            if (pow(circles[i][0] - circles[j][0], 2) + pow(circles[i][1] - circles[j][1], 2) < pow(circles[j][2], 2)) {
                if (!parent[i]) parent[i] = j;

                energy[i] += ++count[i] & 1 ? circles[i][3] : circles[i][4];
                dp[i] = max(dp[i], energy[i]);
            }

        most += dp[i];
        for (int j = count[i], curr = energy[i]; ~j; j--) {
            if (curr == dp[i]) required[i] = j;
            curr -= j & 1 ? circles[i][3] : circles[i][4];
        }
    }

    cout << most << "\n";
    vector<bool> drawn(n, false), skip(n);
    for (int _ = 0; _ < n; _++) {
        fill(skip.begin(), skip.end(), false);
        int i = -1;
        for (int j = 0; j < n; j++) {
            if (skip[j]) skip[parent[j]] = true;
            if (drawn[j] || skip[j]) continue;
            if (energy[j] == dp[j] && (i == -1 || circles[j][5] < circles[i][5])) i = j;
            if (count[j] == required[j]) skip[parent[j]] = true;
        }

        drawn[i] = true;
        for (int j = 0; j < i; j++)
            if (pow(circles[j][0] - circles[i][0], 2) + pow(circles[j][1] - circles[i][1], 2) < pow(circles[i][2], 2))
                energy[j] -= count[j]-- & 1 ? circles[j][3] : circles[j][4];

        cout << circles[i][5] << " ";
    }
}
