#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> grid(n + 2, vector<int>(n + 2));
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++) cin >> grid[i][j];

    auto count = [&](auto &&f) {
        vector<int> histogram(n + 2), pref(n + 2, 0);
        for (int d = 1; d <= n; d++) {
            for (int r = 1; r <= n; r++) {
                histogram[r] = grid[d][r] ? histogram[r] + 1 : 0;
                pref[r] = pref[r - 1] + grid[d + 1][r];
            }

            stack<pair<int, int>> mono;
            for (int r = 0; r <= n; r++) {
                int temp = r + 1;
                while (!mono.empty() && mono.top().first > histogram[r + 1]) {
                    auto [h, l] = mono.top();
                    mono.pop();

                    temp = l;
                    if (pref[r] - pref[l - 1] < r - l + 1) f(d - h + 1, d, l, r);
                }
                if (histogram[r + 1] && (mono.empty() || mono.top().first < histogram[r + 1])) mono.emplace(histogram[r + 1], temp);
            }
        }
    };

    int first = 0, second = 0;
    vector<vector<int>> delta(n + 2, vector<int>(n + 2, 0));
    count([&](int u, int d, int l, int r) {
        first++;
        delta[u][l]++;
        delta[u][r + 1]--;
        delta[d + 1][l]--;
        delta[d + 1][r + 1]++;
    });

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++) delta[i][j] += delta[i - 1][j] + delta[i][j - 1] - delta[i - 1][j - 1];

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++) delta[i][j] = (delta[i][j] == 1) + delta[i - 1][j] + delta[i][j - 1] - delta[i - 1][j - 1];

    count([&](int u, int d, int l, int r) {
        if (delta[d][r] - delta[u - 1][r] > delta[d][l - 1] - delta[u - 1][l - 1]) second++;
    });
    cout << first << " " << second;
}
