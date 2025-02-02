#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    string s;
    cin >> n >> s;

    vector<vector<int>> adj_list(n);
    for (int _ = 0; _ < n - 1; _++) {
        int u, v;
        cin >> u >> v;

        adj_list[u - 1].emplace_back(v - 1);
        adj_list[v - 1].emplace_back(u - 1);
    }

    string path;
    unordered_map<char, char> balance{{'(', ')'}, {'[', ']'}, {'{', '}'}};
    int count = 0;
    for (int i = 0; i < n; i++) {
        vector<bool> visited(n, false);
        auto dfs = [&](auto &&self, int v) -> void {
            visited[v] = true;
            if (balance.count(s[v])) {
                path += s[v];
                for (int u : adj_list[v])
                    if (!visited[u]) self(self, u);
                path.pop_back();
            } else {
                if (path.empty()) return;
                char c = path.back();
                if (balance[c] == s[v]) {
                    path.pop_back();
                    if (path.empty()) count++;
                    for (int u : adj_list[v])
                        if (!visited[u]) self(self, u);
                    path += c;
                }
            }
        };
        dfs(dfs, i);
    }
    cout << count;
}
