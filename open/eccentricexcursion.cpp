#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    k = n - 1 - k;

    vector<vector<bool>> adj_matrix(n, vector<bool>(n, false));
    vector<vector<int>> adj_list(n);
    for (int i = 0; i < n - 1; i++) {
        int a, b;
        cin >> a >> b;

        adj_matrix[a - 1][b - 1] = true;
        adj_matrix[b - 1][a - 1] = true;
        adj_list[a - 1].emplace_back(b - 1);
        adj_list[b - 1].emplace_back(a - 1);
    }

    vector<bool> visited(n, false);
    list<int> cities(n), order;
    iota(cities.begin(), cities.end(), 0);
    auto dfs = [&](auto &&self, int v = 0, int prev = -1) -> void {
        for (int u : adj_list[v])
            if (u != prev) self(self, u, v);
        order.emplace_back(v);
    };
    dfs(dfs);

    vector<list<int>::iterator> cities_iter(n), order_iter(n);
    for (auto it1 = cities.begin(), it2 = order.begin(); it1 != cities.end() && it2 != order.end(); it1++, it2++) {
        cities_iter[*it1] = it1;
        order_iter[*it2] = it2;
    }

    auto range = [&](int c = -1) -> pair<int, int> {
        if (cities.empty()) {
            cout << -1;
            exit(0);
        }

        if (cities.size() == 1) return {0, 0};
        if (cities.size() == 2) {
            auto a = cities.begin(), b = next(a);
            if (adj_matrix[*a][*b]) return {1, 1};
            return {0, 0};
        }

        int l = 0;
        for (int a : cities) {
            if (a == c) continue;

            int f = 0;
            for (int b : cities) {
                if (a == b) continue;

                if (b == c || !adj_matrix[a][b]) f++;
                if (f > (c != -1)) goto next;
            }

            l++;
            break;
            next:;
        }

        if (!l && ~c) {
            for (int a : cities) {
                if (a == c) continue;

                if (!adj_matrix[c][a]) goto done;
            }

            l++;
        }

        done:;
        int r = 0;
        unordered_set<int> asdasd;
        for (int v : order) {
            int roads = 2 - (v == c);

            for (int u : adj_list[v])
                if (!visited[u] && asdasd.count(u)) {
                    asdasd.erase(u);
                    r++;
                    if (!--roads) break;
                }

            if (roads) asdasd.emplace(v);
        }

        return {l, r};
    };
    auto [l, r] = range();
    if (l > k || k > r) {
        cout << -1;
        exit(0);
    }

    vector<int> seq(n, 0);
    for (int s = 0, b = -1; s < n; s++) {
        for (int a = 0; a < n; a++) {
            if (visited[a]) continue;

            int K = k - (~b && adj_matrix[a][b]);
            auto [L, R] = range(a);
            if (L > K || K > R) continue;

            b = a;
            k = K;
            seq[s] = b + 1;
            break;
        }

        if (!seq[s]) {
            cout << -1;
            exit(0);
        }

        cities.erase(cities_iter[b]);
        order.erase(order_iter[b]);
        visited[b] = true;
    }

    for (int city : seq) cout << city << " ";
}
