#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, x;
    cin >> n >> x;

    vector<int> a(n);
    for (int &ai : a) cin >> ai;

    vector<int> root(n, -1), parent(n), depth(n), order;
    for (int s = 0; s < n; s++)
        if (!~root[s]) {
            root[s] = parent[s] = s;
            queue<int> q;
            q.emplace(s);
            while (!q.empty()) {
                int v = q.front();
                q.pop();

                order.emplace_back(v);
                for (int u = 0; u < n; u++)
                    if (!~root[u] && (!(a[v] % a[u]) || !(a[u] % a[v]))) {
                        root[u] = s;
                        q.emplace(u);
                        depth[u] = depth[v] + 1;
                        parent[u] = v;
                    }
            }
        }

    vector<int> sorted(n);
    iota(sorted.begin(), sorted.end(), 0);
    sort(sorted.begin(), sorted.end(), [&](int i, int j) { return a[i] < a[j]; });

    for (int i = 0; i < n; i++)
        if (root[i] != root[sorted[i]]) {
            cout << "NEJ";
            exit(0);
        }

    cout << "JA";
    if (!x) exit(0);

    vector<pair<int, int>> moves;
    vector<int> nodes(n), indices(n);
    iota(nodes.begin(), nodes.end(), 0);
    iota(indices.begin(), indices.end(), 0);
    auto add = [&](int u, int v) {
        int i = indices[u], j = indices[v];
        moves.emplace_back(i + 1, j + 1);
        swap(nodes[i], nodes[j]);
        swap(indices[u], indices[v]);
    };

    vector<int> target(n);
    for (int i = 0; i < n; i++) target[sorted[i]] = i;
    reverse(order.begin(), order.end());
    for (int v : order) {
        int u = nodes[target[v]];
        if (u != v) {
            vector<int> path1, path2;
            while (depth[v] > depth[u]) {
                path1.emplace_back(v);
                v = parent[v];
            }
            while (depth[u] > depth[v]) {
                path2.emplace_back(u);
                u = parent[u];
            }

            while (v != u) {
                path1.emplace_back(v);
                path2.emplace_back(u);
                v = parent[v];
                u = parent[u];
            }
            path1.emplace_back(v);
            reverse(path2.begin(), path2.end());
            path1.insert(path1.end(), path2.begin(), path2.end());
            for (int i = 0; i + 1 < path1.size(); i++) add(path1[i], path1[i + 1]);
            for (int i = path1.size() - 3; ~i; i--) add(path1[i], path1[i + 1]);
        }
    }

    cout << moves.size() << "\n";
    for (auto [i, j] : moves) cout << i << " " << j << "\n";
}
