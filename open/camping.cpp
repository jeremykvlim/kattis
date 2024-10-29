#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> dist(n + 1, vector<int>(m + 1, 0));
    for (int i = 1; i <= n; i++) {
        string s;
        cin >> s;

        for (int j = 1; j <= m; j++) {
            if (s[j - 1] != '.') continue;
            dist[i][j] = min({dist[i - 1][j - 1], dist[i - 1][j], dist[i][j - 1]}) + 1;
        }
    }

    deque<pair<int, int>> dq;
    auto process = [&](bool inv = false) {
        for (int i = 1; i <= (!inv ? n : m); i++) {
            for (int j = (!inv ? m : n); j; j--) {
                int r = !inv ? i : j, c = !inv ? j : i;
                while (!dq.empty() && dq.front().second > j) dq.pop_front();
                while (!dq.empty() && dq.back().first <= dist[r][c]) dq.pop_back();

                if (dq.empty() || dq.back().second > j - dist[r][c] + 1) dq.emplace_back(dist[r][c], j - dist[r][c] + 1);
                dist[r][c] = dq.front().first;
            }
            dq.clear();
        }
    };
    process();
    process(true);
    
    int q;
    cin >> q;

    while (q--) {
        int r, c;
        cin >> r >> c;
        cout << dist[r][c] * dist[r][c] << "\n";
    }
}
