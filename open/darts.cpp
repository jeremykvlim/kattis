#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> values{20, 1, 18, 4, 13, 6, 10, 15, 2, 17, 3, 19, 7, 16, 8, 11, 14, 9, 12, 5};
    vector<vector<double>> dpA(502, vector<double>(502, 0)), dpB(502, vector<double>(502, 0));
    for (int i = 1; i <= 20; i++)
        for (int j = 1; j <= 20; j++) {
            dpA[i][j] = (1 - 19. / 20) / (1 - (19. / 20) * (2. / 3));
            dpB[i][j] = (1 - 2. / 3) / (1 - (19. / 20) * (2. / 3));
        }

    auto A = [&](int i, int j) {
        for (int k = 0; k < 20; k++) dpA[i][j] += (1 - dpB[i >= values[k] ? i - values[k] : i][j]) / 20;
    };

    auto B = [&](int i, int j) {
        for (int k = 0; k < 20; k++) {
            int l = (k + 1) % 20, m = (k + 2) % 20;
            dpB[i][j] = max(dpB[i][j], (1 - dpA[i][j >= values[k] ? j - values[k] : j]
                                        + 1 - dpA[i][j >= values[l] ? j - values[l] : j]
                                        + 1 - dpA[i][j >= values[m] ? j - values[m] : j]) / 3);
        }
    };

    for (int i = 1; i <= 501; i++)
        for (int j = 1; j <= 501; j++) {
            if (i <= 20 && j <= 20) continue;

            if (i <= 20) {
                B(i, j);
                A(i, j);
            } else {
                A(i, j);
                B(i, j);
            }
        }

    int n;
    while (cin >> n && n) cout << fixed << setprecision(8) << dpA[n][n] << " " << dpB[n][n] << "\n";
}
