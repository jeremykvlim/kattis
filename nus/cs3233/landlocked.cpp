#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<string> countries(n);
    for (auto &c : countries) cin >> c;

    vector<vector<bool>> visited(n, vector<bool>(m, false));
    vector<vector<int>> dist(n, vector<int>(m, INT_MAX));
    deque<pair<int, int>> q;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (countries[i][j] == 'W') {
                q.emplace_back(i, j);
                dist[i][j] = -1;
            }


    while (!q.empty()) {
        auto [i, j] = q.front();
        q.pop_front();
        if (visited[i][j]) continue;

        visited[i][j] = true;
        char u = countries[i][j];
        for (int k = -1; k <= 1; k++)
            for (int l = -1; l <= 1; l++)
                if (0 <= i + k && i + k < n && 0 <= j + l && j + l < m) {
                    char v = countries[i + k][j + l];
                    if (u == v) {
                        dist[i + k][j + l] = min(dist[i + k][j + l], dist[i][j]);
                        q.emplace_front(i + k, j + l);
                    } else {
                        dist[i + k][j + l] = min(dist[i + k][j + l], dist[i][j] + 1);
                        q.emplace_back(i + k, j + l);
                    }
                }
    }

    vector<int> letters(26, INT_MAX);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) letters[countries[i][j] - 'A'] = min(letters[countries[i][j] - 'A'], dist[i][j]);

    for (int i = 0; i < 26; i++)
        if (0 <= letters[i] && letters[i] < INT_MAX) cout << (char) ('A' + i) << " " << letters[i] << "\n";
}
