#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    int y_max = -1;
    vector<array<int, 4>> plants(n);
    for (int i = 0; i < n; i++) {
        int x, y, f;
        cin >> x >> y >> f;

        plants[i] = {x, y, f, i};
        y_max = max(y, y_max);
    }
    sort(plants.begin(), plants.end(), [](auto p1, auto p2) {return p1[0] == p2[0] ? p1[1] < p2[1] : p1[0] < p2[0];});

    vector<int> energy(n), next(n, -1);
    vector<pair<int, int>> dp(y_max + 1, {0, 0});
    pair<int, int> most(0, 0);
    for (int i = 0; i < n; i++) {
        if (i && plants[i - 1][0] != plants[i][0]) most = {0, 0};

        if (!plants[i][3]) energy[i] = plants[i][2];
        else {
            auto curr = max(most, dp[plants[i][1]]);
            energy[i] = curr.first - k;

            if (energy[i] >= 0) {
                energy[i] += plants[i][2];
                next[i] = curr.second;
            }
        }

        most = max(most, {energy[i], i});
        dp[plants[i][1]] = max(dp[plants[i][1]], {energy[i], i});
    }

    deque<int> path;
    for (int i = 0; i < n; i++)
        if (plants[i][3] == n - 1) {
            cout << energy[i] << "\n";

            while (i != -1) {
                path.emplace_front(i);
                i = next[i];
            }
            break;
        }

    cout << path.size() << "\n";
    for (int i : path) cout << plants[i][0] << " " << plants[i][1] << "\n";
}
