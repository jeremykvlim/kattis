#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> a(m), b(m);
    for (int i = 0; i < m; i++) cin >> a[i] >> b[i];

    for (int i = 0, smallest = *min_element(a.begin(), a.end()); i < m; i++) {
        a[i] -= smallest;
        b[i] -= smallest;

        if (b[i] < 0) b[i] += n;
    }

    vector<int> count(n + 2, 0);
    vector<vector<pair<int, int>>> adj_list(2 * n + 1);
    for (int i = 0; i < m; i++) {
        count[a[i]]++;
        count[b[i] + 1]--;

        if (a[i] <= b[i]) {
            adj_list[a[i]].emplace_back(b[i] + 1, i);
            adj_list[a[i] + n].emplace_back(b[i] + 1 + n, i);
        } else {
            count[0]++;
            adj_list[a[i]].emplace_back(b[i] + 1 + n, i);
            adj_list[a[i] + n].emplace_back(2 * n, i);
        }
    }

    for (int i = 0; i < n; i++) {
        count[i + 1] += count[i];
        if (count[i] < 2) {
            cout << "impossible";
            exit(0);
        }
        if (count[i] > 2) {
            adj_list[i + 1].emplace_back(i, -1);
            adj_list[i + 1 + n].emplace_back(i + n, -1);
        }
    }

    vector<bool> visited(2 * n + 1, false);
    vector<int> path;
    auto dfs = [&](auto &&self, int v, int start) {
        if (v == start + n) return true;
        if (visited[v]) return false;

        visited[v] = true;
        for (auto [u, i] : adj_list[v]) {
            path.emplace_back(i);
            if (self(self, u, start)) return true;
            path.pop_back();
        }

        return false;
    };

    for (int v = 0; v < n; v++)
        if (dfs(dfs, v, v)) {
            string current(m, '0');
            for (int i : path)
                if (~i) current[i] = '1';
            cout << current;
            exit(0);
        }

    cout << "impossible";
}
