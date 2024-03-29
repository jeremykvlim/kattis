#include <bits/stdc++.h>
using namespace std;

constexpr int MODULO = 9901;

int find(int p, vector<int> &sets) {
    return (sets[p] == p) ? p : (sets[p] = find(sets[p], sets));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> fact(301, 1);
    for (int i = 1; i < 301; i++) fact[i] = (fact[i - 1] * i) % MODULO;

    int t;
    cin >> t;

    for (int x = 1; x <= t; x++) {
        int n, k;
        cin >> n >> k;

        vector<pair<int, int>> edges(k);
        for (auto &[u, v] : edges) cin >> u >> v;

        int y = (fact[n - 1] * (MODULO / 2 + 1)) % MODULO;
        for (int i = 1; i < 1 << k; i++) {
            vector<int> degree(n + 1, 0), degree_count(4, 0), sets(n + 1);
            iota(sets.begin(), sets.end(), 0);

            int cycles = 0;
            for (int j = 0; j < k; j++)
                if (i & (1 << j)) {
                    auto [u, v] = edges[j];
                    degree[u]++;
                    degree[v]++;

                    int u_set = find(u, sets), v_set = find(v, sets);
                    sets[v_set] = sets[u_set];
                    if (u_set == v_set) cycles++;
                }

            int forbidden = __builtin_popcount(i);
            for (int j = 1; j <= n; j++) degree_count[min(degree[j], 3)]++;

            if (degree_count[3]) continue;
            else if (cycles) {
                if (cycles == 1 && forbidden == n) y += forbidden & 1 ? -1 : 1;
                continue;
            }

            y += (forbidden & 1 ? -1 : 1) * (fact[n - 1 - forbidden] << (degree_count[1] / 2 - 1)) % MODULO;
        }

        cout << "Case #" << x << ": " << (y % MODULO + MODULO) % MODULO << "\n";
    }
}
