#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--) {
        int n, m, i;
        cin >> n >> m >> i;
        cin.ignore();

        vector<vector<int>> needs(i), creates(i);
        for (int j = 0; j < 2 * n; j++) {
            string s;
            getline(cin, s);

            istringstream iss(s);
            vector<int> ideas{istream_iterator<int>{iss}, istream_iterator<int>{}};
            for (int id : ideas) {
                if (!(j & 1)) needs[id].emplace_back(j / 2);
                else creates[id].emplace_back(j / 2);
            }
        }

        vector<vector<pair<int, int>>> adj_list(n);
        for (int j = 0; j < m; j++) {
            int u, v;
            cin >> u >> v;

            adj_list[v].emplace_back(u, j);
        }

        vector<vector<int>> tubes(m);
        for (int id = 0; id < i; id++) {
            queue<int> q;
            vector<bool> visited(n, false), created(n, false), seen(m, false);
            for (int j : needs[id]) {
                q.emplace(j);
                visited[j] = true;
            }
            for (int j : creates[id]) created[j] = true;

            while (!q.empty()) {
                int v = q.front();
                q.pop();

                for (auto [u, j] : adj_list[v]) {
                    seen[j] = true;

                    if (!visited[u] && !created[u]) {
                        q.emplace(u);
                        visited[u] = true;
                    }
                }
            }

            for (int j = 0; j < m; j++)
                if (seen[j]) tubes[j].emplace_back(id);
        }

        for (auto t : tubes) {
            for (int id : t) cout << id << " ";
            cout << "\n";
        }
    }
}
