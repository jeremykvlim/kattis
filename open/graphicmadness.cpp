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
            auto [v, u] = read();
            adj_list[v].emplace_back(u);
            adj_list[u].emplace_back(v);
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

            vector<int> p(n + k + m + k, -1), c(n + k + m + k, -1);
            auto connect = [&](int v, int u) -> bool {
                if (p[v] == -1) p[v] = u;
                else if (c[v] == -1) c[v] = u;
                else return false;

                return true;
            };

            for (auto [v, u] : path)
                if (!connect(v, u) || !connect(u, v)) return false;

            int prev = c[0], curr = 0, next;
            do {
                indices.emplace_back(curr);
                next = prev ^ p[curr] ^ c[curr];
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
