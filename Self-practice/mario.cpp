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
        vector<bool> visited(n, false), right(n, true);
        vector<int> position(n);
        for (int i = 0; i < n; i++) {
            cin >> boats[i].first >> boats[i].second;
            position[i] = boats[i].first;
            visited[i] = (!position[i]);
        }

        for (int time = 1; time <= w * w + 1; time++) {
            unordered_map<int, int> mario;
            vector<int> prev(position);
            for (int i = 0; i < n; i++) {
                position[i] += right[i] ? 1 : -1;
                right[i] = position[i] != boats[i].second && ((position[i] == boats[i].first) || right[i]);

                if (position[i] == w && visited[i]) {
                    cout << time << '\n';
                    goto next;
                }
                if (visited[i]) mario[position[i]] = prev[i];
            }

            for (int i = 0; i < n; i++)
                visited[i] = visited[i] || mario.count(position[i]) || (mario.count(prev[i]) && mario[prev[i]] == position[i]);
        }
        cout << "IMPOSSIBLE\n";
        next:;
    }
}
