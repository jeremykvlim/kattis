#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int k, n, m;
        cin >> k >> n >> m;

        auto read = [&]() -> pair<int, int> {
            string node1, node2;
            cin >> node1 >> node2;

            auto index = [&](string node) -> int {
                int i = stoi(node.substr(2)) - 1;

                if (node[0] == 'A' && node[1] == 'P') return i;
                if (node[0] == 'A' && node[1] == 'S') return i + n;
                if (node[0] == 'B' && node[1] == 'P') return i + n + k;
                if (node[0] == 'B' && node[1] == 'S') return i + n + k + m;
            };

            return {index(node1), index(node2)};
        };

        vector<vector<int>> adj_list(n + k + m + k);
        for (int i = 0; i < n + k + m + k - 2; i++) {
            auto [u, v] = read();
            
            adj_list[u].emplace_back(v);
            adj_list[v].emplace_back(u);
        }

        vector<pair<int, int>> path;
        for (int i = 0; i < k; i++) path.emplace_back(read());

        auto dfs = [&](auto &&self, int v, int prev = -1) -> bool {
            bool pendant = adj_list[v].size() == 1;
            
            for (int u : adj_list[v])
                if (u != prev && self(self, u, v)) {
                    path.emplace_back(v, u);
                    pendant = !pendant;
                }

            return pendant;
        };

        vector<int> indices;
        auto valid = [&]() -> bool {
            if (path.size() != n + k + m + k) return false;

            vector<int> child(n + k + m + k, -1), parent(n + k + m + k, -1);
            auto connect = [&](int v, int u) -> bool {
                if (child[v] == -1) child[v] = u;
                else if (parent[v] == -1) parent[v] = u;
                else return false;

                return true;
            };

            for (auto [u, v] : path)
                if (!connect(u, v) || !connect(v, u)) return false;

            int prev = parent[0], curr = 0, next;
            do {
                indices.emplace_back(curr);
                next = prev ^ child[curr] ^ parent[curr];
                prev = curr;
                curr = next;
            } while (curr);

            return indices.size() == n + k + m + k;
        };

        if (dfs(dfs, 0) || dfs(dfs, n + k) || !valid()) cout << "NO\n";
        else {
            cout << "YES";

            auto node = [&](int i) -> string {
                if (i <= n) return "AP" + to_string(i);
                if (i <= n + k) return "AS" + to_string(i - n);
                if (i <= n + k + m) return "BP" + to_string(i - n - k);
                if (i <= n + k + m + k) return "BS" + to_string(i - n - k - m);
            };

            for (int i = 0; i < n + k + m + k; i++) cout << " " << node(indices[i] + 1);
            cout << "\n";
        }
    }
}
