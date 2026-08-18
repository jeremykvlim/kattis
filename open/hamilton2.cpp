#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, t;
    cin >> n >> t;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            bool b = false;
            if (i < j) b = 2 * (j - i) <= n;
            else if (i > j) b = 2 * (i - j) > n;
            cout << b;
        }
        cout << "\n";
    }
    cout << flush;

    mt19937 rng(random_device{}());
    while (t--) {
        vector<vector<int>> adj_matrix(n + 1, vector<int>(n + 1, -1));
        auto query = [&](int u, int v) {
            if (!~adj_matrix[u][v]) {
                cout << "? " << u << " " << v << "\n" << flush;

                char c;
                cin >> c;

                adj_matrix[u][v] = c == '>';
                adj_matrix[v][u] = c != '>';
            }
            return adj_matrix[u][v];
        };

        vector<vector<int>> paths;
        for (int i = 1; i < n; i += 2)
            if (query(i, i + 1)) paths.push_back({i, i + 1});
            else paths.push_back({i + 1, i});
        if (n & 1) paths.push_back({n});

        while (paths.size() > 1) {
            int i = 1;
            for (; i < paths.size() && !query(paths[0].back(), paths[i].front()); i++);

            if (i < paths.size()) {
                paths[0].insert(paths[0].end(), paths[i].begin(), paths[i].end());
                paths.erase(paths.begin() + i);
                shuffle(paths.begin() + i, paths.end(), rng);
            } else {
                int m = rng() % paths[0].size();
                vector<int> p(paths[0].begin(), paths[0].begin() + m), q(paths[0].begin() + m, paths[0].end());
                paths.erase(paths.begin());
                if (!p.empty()) paths.emplace_back(p);
                if (!q.empty()) paths.emplace_back(q);
                shuffle(paths.begin(), paths.end(), rng);
            }
        }

        auto path = paths[0];
        int k = path.size();
        for (; !query(path[k - 1], path[0]); k--);

        vector<int> cycle(path.begin(), path.begin() + k);
        for (int i = cycle.size() - 1; k < n; k++)
            for (;;) {
                int j = (i + 1) % cycle.size();
                if (query(path[k], cycle[j])) {
                    cycle.insert(cycle.begin() + i + 1, path[k]);
                    i = j;
                    break;
                }
                i = j;
            }

        cout << "! ";
        for (int v : cycle) cout << v << " ";
        cout << "\n" << flush;
    }
}
