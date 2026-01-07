#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    string s;
    cin >> n >> q >> s;

    vector<vector<int>> pref(n + 1, vector<int>(26, 0)), prev(n + 1, vector<int>(26, 0)), count(n + 1, vector<int>(26, 0));
    for (int i = 1; i <= n; i++) {
        pref[i] = pref[i - 1];
        prev[i] = prev[i - 1];
        count[i] = count[i - 1];

        int c1 = s[i - 1] - 'a';
        pref[i][c1]++;
        if (i < n) {
            int c2 = s[i] - 'a';
            if (prev[i][c1] != c2) {
                prev[i][c1] = c2;
                count[i][c1] = 0;
            }
            count[i][c1]++;
        }
    }

    while (q--) {
        int l, r;
        cin >> l >> r;

        int f = 0;
        vector<int> freq(26, 0);
        for (int c = 0; c < 26; c++) f = max(f, freq[c] = pref[r][c] - pref[l - 1][c]);

        if (f == 1) {
            cout << r - l + 1 << "\n";
            continue;
        }

        int edge_mask = 0;
        for (int c = 0; c < 26; c++)
            if (c != s[r - 1] - 'a' && count[r][c] >= f) edge_mask |= 1 << c;

        vector<int> dist(26, 1);
        vector<bool> visited(26, false);
        auto dfs = [&](auto &&self, int v) -> void {
            visited[v] = true;
            if ((edge_mask >> v) & 1) {
                int u = prev[r][v];
                if (!visited[u]) self(self, u);
                dist[v] = max(dist[v], dist[u] + 1);
            }
        };
        for (int c = 0; c < 26; c++)
            if (freq[c] == f && !visited[c]) dfs(dfs, c);
        cout << *max_element(dist.begin(), dist.end()) << "\n";
    }
}
