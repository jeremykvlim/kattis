#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, w;
        cin >> n >> w;

        vector<pair<int, int>> boats(n);
        vector<bool> visited(n, false), r(n, true);
        vector<int> x(n);
        for (int i = 0; i < n; i++) {
            cin >> boats[i].first >> boats[i].second;

            x[i] = boats[i].first;
            visited[i] = !x[i];
        }

        for (int time = 1; time <= w * w; time++) {
            unordered_map<int, int> mario;
            vector<int> prev(x);
            for (int i = 0; i < n; i++) {
                x[i] += r[i] ? 1 : -1;
                r[i] = x[i] != boats[i].second && ((x[i] == boats[i].first) || r[i]);

                if (x[i] == w && visited[i]) {
                    cout << time << "\n";
                    goto next;
                }

                if (visited[i]) mario[x[i]] = prev[i];
            }

            for (int i = 0; i < n; i++)
                if (mario.count(x[i]) || (mario.count(prev[i]) && mario[prev[i]] == x[i])) visited[i] = true;
        }
        cout << "IMPOSSIBLE\n";
        next:;
    }
}
