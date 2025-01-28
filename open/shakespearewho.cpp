#include <bits/stdc++.h>
using namespace std;

vector<int> prefix_function(const string &s) {
    vector<int> pi(s.size());
    for (int i = 1; i < s.size(); i++) {
        int j = pi[i - 1];
        while (j && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }

    return pi;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<string> words(n);
    for (auto &w : words) cin >> w;

    vector<vector<int>> adj_matrix(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) {
            adj_matrix[i][j] = prefix_function(words[i] + words[j]).back();
            adj_matrix[j][i] = prefix_function(words[j] + words[i]).back();
        }

    vector<bool> visited(n);
    deque<int> order;
    auto dfs = [&](auto &&self, int v) -> void {
        visited[v] = true;
        for (int u = 0; u < adj_matrix.size(); u++)
            if (u != v && !visited[u] && adj_matrix[v][u]) self(self, u);

        order.emplace_front(v);
    };
    for (int i = 0; i < n; i++)
        if (!visited[i]) dfs(dfs, i);

    vector<int> indices(n);
    for (int i = 0; i < n; i++) indices[order[i]] = i;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (i != j && adj_matrix[i][j] && indices[i] > indices[j]) {
                cout << "Shakespeare, who?";
                exit(0);
            }

    vector<int> dp(n, 0);
    for (int i : order)
        for (int j = 0; j < n; j++)
            if (i != j && adj_matrix[i][j]) dp[j] = max(dp[j], dp[i] + adj_matrix[i][j]);

    cout << *max_element(dp.begin(), dp.end());
}
