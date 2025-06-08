#include <bits/stdc++.h>
using namespace std;

template <typename T>
pair<int, vector<list<pair<int, int>>>> additive_phylogeny(vector<vector<T>> D) {
    int n = D.size();
    vector<list<pair<int, T>>> adj_list(2 * n);
    vector<vector<typename list<pair<int, T>>::iterator>> its(2 * n, vector<typename list<pair<int, T>>::iterator>(2 * n));

    auto add = [&](int u, int v, T w) {
        adj_list[u].emplace_back(v, w);
        adj_list[v].emplace_back(u, w);
        its[u][v] = prev(adj_list[u].end());
        its[v][u] = prev(adj_list[v].end());
    };

    auto remove = [&](int u, int v) {
        adj_list[u].erase(its[u][v]);
        adj_list[v].erase(its[v][u]);
    };

    vector<int> leaves(n);
    iota(leaves.begin(), leaves.end(), 0);
    auto build = [&](auto &&self) {
        int m = leaves.size();
        if (m == 2) {
            int i = leaves[0], j = leaves[1];
            add(i + 1, j + 1, D[i][j]);
            return;
        }

        T limb = numeric_limits<T>::max();
        int j = leaves[m - 1];
        for (int l1 = 0; l1 < m - 1; l1++)
            for (int l2 = l1 + 1; l2 < m - 1; l2++) {
                int i = leaves[l1], k = leaves[l2];
                limb = min(limb, (D[i][j] + D[j][k] - D[i][k]) / 2);
            }

        for (int l = 0; l < m - 1; l++) {
            int i = leaves[l];
            D[i][j] -= limb;
            D[j][i] -= limb;
        }

        T d = 0;
        int i = -1, k = -1;
        for (int l1 = 0; l1 < m - 1; l1++)
            for (int l2 = 0; l2 < m - 1; l2++)
                if (l1 != l2) {
                    i = leaves[l1], k = leaves[l2];
                    if (D[i][k] == D[i][j] + D[j][k]) {
                        d = D[i][j];
                        goto done;
                    }
                }
        done:;

        leaves.pop_back();
        self(self);

        if (!d) {
            add(i + 1, j + 1, limb);
            return;
        }

        vector<tuple<int, int, T>> path;
        auto dfs = [&](auto &&self, int v, int prev = -1) {
            if (v == k + 1) return true;
            for (auto [u, w] : adj_list[v])
                if (u != prev) {
                    path.emplace_back(v, u, w);
                    if (self(self, u, v)) return true;
                    path.pop_back();
                }
            return false;
        };
        dfs(dfs, i + 1);

        T dist = 0;
        for (auto [u, v, w] : path) {
            if (dist + w == d) {
                add(v, j + 1, limb);
                return;
            }

            if (dist + w > d) {
                remove(u, v);
                n++;
                add(u, n, d - dist);
                add(n, v, dist + w - d);
                add(n, j + 1, limb);
                return;
            }

            dist += w;
        }
    };
    build(build);
    return {n, adj_list};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k;
    cin >> k;

    vector<vector<int>> d(k, vector<int>(k));
    for (auto &row : d)
        for (int &dij : row) cin >> dij;

    auto [G, adj_list] = additive_phylogeny(d);

    vector<pair<int, int>> trails;
    vector<vector<bool>> visited(G + 1, vector<bool>(G + 1, false));
    for (int a = 1; a < 2 * k; a++)
        for (auto [b, w] : adj_list[a])
            if (a < b && !visited[a][b] && !visited[b][a]) {
                visited[a][b] = visited[b][a] = true;
                if (w == 1) trails.emplace_back(a, b);
                else {
                    int c = a;
                    for (int _ = 0; _ < w - 1; _++) {
                        G++;
                        trails.emplace_back(c, G);
                        c = G;
                    }
                    trails.emplace_back(c, b);
                }
            }

    cout << G << "\n";
    for (auto [a, b] : trails) cout << a << " " << b << "\n";
}
