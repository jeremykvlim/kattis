#include <bits/stdc++.h>
using namespace std;

template <typename T>
pair<T, T> bezout(T a, T b) {
    if (!a) return {0, 1};
    auto [x, y] = bezout(b % a, a);
    return {y - (b / a) * x, x};
}

template <typename T>
T linear_congruence_solution(T a, T b, T n) {
    T g = __gcd(a, n);
    if (b % g) return -1;

    a /= g;
    b /= g;
    n /= g;
    auto [x, y] = bezout(a, n);
    x = (x + n) % n;
    return (b * x) % n;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int V, E;
    long long K;
    cin >> V >> E >> K;

    vector<array<int, 3>> edges(E);
    vector<vector<pair<int, int>>> adj_list(V + 1);
    vector<vector<int>> dist(V + 1, vector<int>(V + 1, 1e9));
    for (int i = 1; i <= V; i++) dist[i][i] = 0;
    for (auto &[u, v, w] : edges) {
        cin >> u >> v >> w;

        dist[u][v] = min(dist[u][v], w);
        dist[v][u] = min(dist[v][u], w);
        adj_list[u].emplace_back(v, w);
        adj_list[v].emplace_back(u, w);
    }

    for (int i = 1; i <= V; i++)
        for (int j = 1; j <= V; j++)
            for (int k = 1; k <= V; k++) dist[j][k] = min(dist[j][k], dist[j][i] + dist[i][k]);

    int count = 0;
    vector<bool> visited(V + 1, false);
    vector<int> component(V + 1, 0), component_gcd(V + 1, 0);
    for (int v = 1; v <= V; v++)
        if (!visited[v]) {
            visited[v] = true;
            component[v] = ++count;
            vector<int> depth(V + 1, 0);
            vector<pair<int, int>> parent(V + 1, {-1, -1});
            queue<int> q;
            q.emplace(v);
            while (!q.empty()) {
                int v = q.front();
                q.pop();

                for (auto [u, w] : adj_list[v])
                    if (!visited[u]) {
                        visited[u] = true;
                        component[u] = count;
                        parent[u] = {v, w};
                        depth[u] = depth[v] + 1;
                        q.emplace(u);
                    }
            }

            for (auto [x, y, z] : edges)
                if (component[x] == count && component[y] == count) {
                    component_gcd[count] = !component_gcd[count] ? 2 * z : __gcd(component_gcd[count], 2 * z);
                    if (parent[x] != make_pair(y, z) && parent[y] != make_pair(x, z)) {
                        int sum = 0;
                        while (x != y)
                            if (depth[x] > depth[y]) {
                                auto [u, w] = parent[x];
                                sum += w;
                                x = u;
                            } else if (depth[y] > depth[x]) {
                                auto [u, w] = parent[y];
                                sum += w;
                                y = u;
                            } else {
                                auto [ux, wx] = parent[x];
                                auto [uy, wy] = parent[y];
                                sum += wx + wy;
                                x = ux;
                                y = uy;
                            }
                        component_gcd[count] = !component_gcd[count] ? sum + z : __gcd(component_gcd[count], sum + z);
                    }
                }
        }
    vector<int> gcd(V + 1, 0);
    for (int i = 1; i <= V; i++) gcd[i] = component_gcd[component[i]];

    for (int i = 1; i <= V; i++) {
        for (int j = 1; j <= V; j++)
            if (dist[i][j] == 1e9) cout << "-1 ";
            else {
                auto len = [&](__int128 d, __int128 g) -> __int128 {
                    if (!g) return d % K ? -1 : d;

                    auto r = (d + K) % K;
                    if (!r) return d;

                    auto x = linear_congruence_solution(g, (__int128) K - r, (__int128) K);
                    if (x == -1) return -1;
                    return d + g * x;
                };

                auto l = len(dist[i][j], gcd[i]);
                if (l < 1e7) cout << (int) l << " ";
                else cout << setw(7) << setfill('0') << (int) (l % (int) 1e7) << " ";
            }
        cout << "\n";
    }
}
