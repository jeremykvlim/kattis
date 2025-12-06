#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    string color(m, '#');
    vector<pair<int, int>> edges(m);
    vector<vector<pair<int, int>>> adj_list(n + 1);
    for (int i = 0; i < m; i++) {
        auto &[x, y] = edges[i];
        char c;
        cin >> x >> y >> c;
        if (!~x) x = 0;
        if (!~y) y = 0;

        color[i] = c;
        adj_list[x].emplace_back(y, i);
        adj_list[y].emplace_back(x, i);
    }

    vector<int> memo1(1 << (m + 1), -1);
    auto prune = [&](int mask) -> int {
        if (~memo1[mask]) return memo1[mask];

        vector<bool> visited(n + 1, false);
        visited[0] = true;
        queue<int> q;
        q.emplace(0);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (auto [u, i] : adj_list[v])
                if ((mask >> i) & 1)
                    if (!visited[u]) {
                        visited[u] = true;
                        q.emplace(u);
                    }
        }

        memo1[mask] = 0;
        for (int i = 0; i < m; i++)
            if (((mask >> i) & 1) && visited[edges[i].first] && visited[edges[i].second]) memo1[mask] |= 1 << i;
        return memo1[mask];
    };

    vector<pair<long long, int>> memo2(1 << (m + 1), {-1, -1});
    auto dfs = [&](auto &&self, int m0) -> pair<long long, int> {
        if (memo2[m0] != make_pair(-1LL, -1)) return memo2[m0];
        if (!m0) return memo2[m0] = {0, 0};

        vector<bool> visited(n + 1, false);
        vector<int> component_masks;
        for (int s = 1; s <= n; s++) {
            if (visited[s] || !any_of(adj_list[s].begin(), adj_list[s].end(), [&](auto e) { return (m0 >> e.second) & 1; })) continue;

            int m1 = 0;
            visited[s] = true;
            queue<int> q;
            q.emplace(s);
            while (!q.empty()) {
                int v = q.front();
                q.pop();

                for (auto [u, i] : adj_list[v])
                    if ((m0 >> i) & 1) {
                        m1 |= 1 << i;
                        if (u && !visited[u]) {
                            visited[u] = true;
                            q.emplace(u);
                        }
                    }
            }
            component_masks.emplace_back(m1);
        }

        auto scale = [&](long long p, int q, int Q) -> long long {
            if (q >= Q) return p >> (q - Q);
            return p << (Q - q);
        };

        if (component_masks.size() > 1) {
            auto p0 = 0LL;
            int q0 = 0;
            for (int m1 : component_masks) {
                auto [p1, q1] = self(self, m1);
                int Q = max(q0, q1);
                p0 = scale(p0, q0, Q) + scale(p1, q1, Q);
                q0 = Q;
            }
            return memo2[m0] = {p0, q0};
        }

        auto cmp = [&](long long p1, int q1, long long p2, int q2) -> long long {
            int Q = max(q1, q2);
            return scale(p1, q1, Q) - scale(p2, q2, Q);
        };

        bool x = false, y = false;
        auto px = 0LL, py = 0LL;
        int qx = 0, qy = 0;
        for (int m1 = m0; m1;) {
            int lsb = m1 & -m1;
            m1 ^= lsb;
            auto [p, q] = self(self, prune(m0 ^ lsb));
            if (color[countr_zero((unsigned) lsb)] == 'B') {
                if (!x || cmp(p, q, px, qx) > 0) {
                    px = p;
                    qx = q;
                    x = true;
                }
            } else {
                if (!y || cmp(p, q, py, qy) < 0) {
                    py = p;
                    qy = q;
                    y = true;
                }
            }
        }

        if (x && y) {
            int l = -1, r = max(qx, qy) + 1, mid;
            while (l + 1 < r) {
                mid = l + (r - l) / 2;

                if (scale(px, qx, mid) + 1 < -scale(-py, qy, mid)) r = mid;
                else l = mid;
            }
            return memo2[m0] = {scale(px, qx, r) + 1, r};
        }

        if (x) {
            if (px < 0) return memo2[m0] = {0, 0};
            return memo2[m0] = {scale(px, qx, 0) + 1, 0};
        }

        if (y) {
            if (py > 0) return memo2[m0] = {0, 0};
            auto P = scale(py, qy, 0);
            return memo2[m0] = {P - (!qy && P == py), 0};
        }
        return memo2[m0] = {0, 0};
    };
    auto [P, Q] = dfs(dfs, prune((1 << m) - 1));
    for (; Q && !(P & 1); P >>= 1, Q--);

    if (!Q) cout << P;
    else cout << P << "/2^" << Q;
}
