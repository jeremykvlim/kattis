#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<string> words(n);
    for (auto &w : words) cin >> w;

    vector<string> a(n);
    for (auto &w : a) {
        int i;
        cin >> i;

        w = words[i - 1];
    }

    for (int i = 0; i < n; i++)
        for (int j = 0; j < i; j++)
            if (!a[j].compare(0, a[i].size(), a[i])) {
                cout << "NE";
                exit(0);
            }

    vector<vector<int>> adj_list(26);
    vector<int> degree(26, 0);
    for (int i = 0, j = 1; i < n - 1 && j < n; i++, j++)
        for (int k = 0; k < min(a[i].size(), a[j].size()); k++)
            if (a[i][k] != a[j][k]) {
                int c1 = a[i][k] - 'a', c2 = a[j][k] - 'a';
                adj_list[c2].emplace_back(c1);
                degree[c1]++;
                break;
            }

    vector<int> state(26, 0);
    auto dfs1 = [&](auto &&self, int v) -> bool {
        state[v] = 1;
        for (int u : adj_list[v])
            if (!state[u]) {
                if (self(self, u)) return true;
            } else if (state[u] == 1) return true;

        state[v] = 2;
        return false;
    };

    for (int i = 0; i < 26; i++)
        if (dfs1(dfs1, i)) {
            cout << "NE";
            exit(0);
        }

    vector<bool> visited(26, false);
    vector<int> order;
    auto dfs2 = [&](auto &&self, int v) -> void {
        visited[v] = true;
        for (int u : adj_list[v])
            if (!visited[u]) self(self, u);

        order.emplace_back(v);
    };
    for (int i = 0; i < 26; i++)
        if (!degree[i] && !visited[i]) dfs2(dfs2, i);

    string key("", 26);
    for (int i = 0; i < 26; i++) key[order[i]] = (char) (i + 'a');
    cout << "DA\n" << key;
}
