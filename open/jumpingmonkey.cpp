#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, t = 0;
    vector<int> visited(1 << 21, -1), prev(1 << 21), indices(1 << 21);
    while (cin >> n >> m && n || m) {
        vector<int> adj_masks(n, 0);
        while (m--) {
            int u, v;
            cin >> u >> v;

            adj_masks[u] |= 1 << v;
            adj_masks[v] |= 1 << u;
        }

        int s = (1 << n) - 1;
        visited[s] = t;
        queue<int> q;
        q.emplace(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            vector<int> pref_masks(n, 0);
            for (int i = 0, mask = 0; i < n; i++) {
                if ((v >> i) & 1) mask |= adj_masks[i];
                pref_masks[i] = mask;
            }

            vector<int> suff_masks(n, 0);
            for (int i = n - 1, mask = 0; ~i; i--) {
                if ((v >> i) & 1) mask |= adj_masks[i];
                suff_masks[i] = mask;
            }

            for (int i = 0; i < n; i++)
                if ((v >> i) & 1) {
                    int u = 0;
                    if (i) u |= pref_masks[i - 1];
                    if (i + 1 < n) u |= suff_masks[i + 1];

                    if (visited[u] != t) {
                        visited[u] = t;
                        prev[u] = v;
                        indices[u] = i;
                        if (!u) goto done;
                        q.emplace(u);
                    }
                }
        }

        done:;
        if (visited[0] != t) cout << "Impossible\n";
        else {
            vector<int> trees;
            for (int v = 0; v != s; v = prev[v]) trees.emplace_back(indices[v]);
            reverse(trees.begin(), trees.end());

            cout << trees.size() << ": ";
            for (int v : trees) cout << v << " ";
            cout << "\n";
        }
        t++;
    }
}
