#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k, Q;
    cin >> n >> k >> Q;

    vector<vector<pair<int, bool>>> adj_list(n + 1);
    for (int _ = 0; _ < n - 1; _++) {
        int a, b;
        cin >> a >> b;

        adj_list[a].emplace_back(b, true);
        adj_list[b].emplace_back(a, false);
    }

    queue<int> q;
    vector<bool> known(n + 1, false), visited(n + 1, false);
    while (k--) {
        int f;
        cin >> f;

        known[f] = true;
        if (!visited[f]) {
            visited[f] = true;
            q.emplace(f);
        }
    }

    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (auto [u, out] : adj_list[v])
            if (!visited[u] && !out) {
                visited[u] = true;
                q.emplace(u);
            }
    }

    auto lsb = [&](unsigned x) {
        return x & -x;
    };

    vector<pair<int, int>> tour;
    vector<int> child(n + 1, 0), depth(n + 1, 0), inlabel(n + 1), ascendant(n + 1, 0), head(n + 2);
    vector<bool> down(n + 1);
    auto dfs = [&](auto &&self, int v = 1, int prev = 1) -> void {
        tour.emplace_back(v, prev);
        inlabel[v] = tour.size();

        for (auto [u, out] : adj_list[v])
            if (u != prev) {
                depth[u] = depth[v] + 1;
                down[u] = out;
                self(self, u, v);
                head[inlabel[u]] = v;
                if (lsb(inlabel[v]) < lsb(inlabel[u])) inlabel[v] = inlabel[u];
            }
    };
    dfs(dfs);

    for (auto [v, p] : tour) {
        ascendant[v] = ascendant[p] | lsb(inlabel[v]);
        if (v && inlabel[v] == inlabel[p]) child[p] = v;
    }

    auto lca = [&](int u, int v) {
        if (unsigned above = inlabel[u] ^ inlabel[v]; above) {
            above = (ascendant[u] & ascendant[v]) & -bit_floor(above);

            if (unsigned below = ascendant[u] ^ above; below) {
                below = bit_floor(below);
                u = head[(inlabel[u] & -below) | below];
            }

            if (unsigned below = ascendant[v] ^ above; below) {
                below = bit_floor(below);
                v = head[(inlabel[v] & -below) | below];
            }
        }

        return depth[u] < depth[v] ? u : v;
    };

    vector<int> pref(n + 1, 0);
    for (auto [v, p] : tour)
        if (v != 1 && p != 1) pref[v] = pref[p] + (!down[v] && down[p] ? !visited[p] : known[p]);

    vector<int> base, offset(n + 1), pos(n + 1);
    for (auto [v, p] : tour)
        if (v == 1 || inlabel[v] != inlabel[p]) {
            offset[inlabel[v]] = base.size();
            for (int u = v; u; u = child[u]) {
                base.emplace_back(u);
                pos[u] = base.size() - 1;
            }
        }

    auto climb = [&](int v, int d) -> pair<int, bool> {
        int u = v;
        while (d) {
            if (d <= pos[v] - offset[inlabel[v]]) {
                v = base[pos[v] - d];
                return {v, pref[u] == pref[v]};
            }
            d -= pos[v] - offset[inlabel[v]] + 1;
            v = head[inlabel[v]];
        }
        return {v, pref[u] == pref[v]};
    };

    while (Q--) {
        int c, d;
        cin >> c >> d;

        int a = lca(c, d), dc = depth[c] - depth[a], dd = depth[d] - depth[a], cc = -1, cd = -1;
        if (dc) {
            auto [v, same] = climb(c, dc - 1);
            if (!same) {
                cout << "0\n";
                continue;
            }
            cc = v;
        }

        if (dd) {
            auto [v, same] = climb(d, dd - 1);
            if (!same) {
                cout << "0\n";
                continue;
            }
            cd = v;
        }

        if (dc && dd && (!down[cc] && !down[cd] ? !visited[a] : known[a])) {
            cout << "0\n";
            continue;
        }

        cout << "1\n";
    }
}
