#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, Q, k;
    cin >> n >> Q >> k;

    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];

    vector<vector<int>> adj_list(n + 1);
    for (int i = 2; i <= n; i++) {
        int p;
        cin >> p;

        adj_list[p].emplace_back(i);
        adj_list[i].emplace_back(p);
    }

    vector<int> prev(n + 1, 0), depth(n + 1, 0), heavy(n + 1, -1);
    auto hld = [&](auto &&self, int v = 1) -> int {
        int subtree_size = 1, largest = 0;
        for (int u : adj_list[v])
            if (u != prev[v]) {
                prev[u] = v;
                depth[u] = depth[v] + 1;
                int size = self(self, u);
                subtree_size += size;
                if (largest < size) {
                    largest = size;
                    heavy[v] = u;
                }
            }
        return subtree_size;
    };
    hld(hld);

    int lg = __lg(n) + 1;
    vector<vector<int>> lift(lg, vector<int>(n + 1, 0));
    for (int v = 1; v <= n; v++) lift[0][v] = prev[v];
    for (int i = 1; i < lg; i++)
        for (int v = 1; v <= n; v++) lift[i][v] = lift[i - 1][lift[i - 1][v]];

    auto lca = [&](int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);

        int diff = depth[u] - depth[v];
        for (int i = 0; i < lg; i++)
            if ((diff >> i) & 1) u = lift[i][u];

        if (u == v) return u;

        for (int i = lg - 1; ~i; i--)
            if (lift[i][u] != lift[i][v]) {
                u = lift[i][u];
                v = lift[i][v];
            }

        return lift[0][u];
    };

    auto dist = [&](int u, int v) {
        return depth[u] + depth[v] - 2 * depth[lca(u, v)];
    };

    int count = 0, size = 0;
    vector<int> head(n + 1, 0), chain_id(n + 1, -1), chain_pos(n + 1, -1);
    vector<vector<int>> chains;
    queue<int> q;
    q.emplace(1);
    while (!q.empty()) {
        int h = q.front();
        q.pop();

        chains.emplace_back();
        for (int v = h; ~v; v = heavy[v]) {
            chain_id[v] = count;
            chains[count].emplace_back(v);
            chain_pos[v] = chains[count].size() - 1;
            head[v] = h;
            for (int u : adj_list[v])
                if (u != prev[v] && u != heavy[v]) q.emplace(u);
        }
        size = max(size, (int) chains.back().size());
        count++;
    }

    int lg_len = __lg(size) + 1;
    vector<vector<int>> block_count(lg_len + 1, vector<int>(count, 0)), pref(lg_len + 1, vector<int>(count + 1, 0));
    for (int l = 0; l <= lg_len; l++)
        for (int c = 0, s = 1 << l; c < count; c++) {
            if (chains[c].size() >= s) block_count[l][c] = chains[c].size() - s + 1;
            pref[l][c + 1] = pref[l][c] + 2 * block_count[l][c];
        }

    auto encode = [&](int l, int c, int p, int dir) {
        return pref[l][c] + p + dir * block_count[l][c];
    };

    auto decode = [&](int l, int id) -> array<int, 3> {
        int c = (upper_bound(pref[l].begin(), pref[l].end(), id) - pref[l].begin()) - 1,
            p = id - pref[l][c],
            dir = p >= block_count[l][c];

        if (dir) p -= block_count[l][c];
        return {c, p, dir};
    };

    auto decompose = [&](int u, int v) {
        vector<array<int, 4>> l, r;
        while (head[u] != head[v])
            if (depth[head[u]] >= depth[head[v]]) {
                l.push_back({chain_id[u], chain_pos[head[u]], chain_pos[u], 1});
                u = prev[head[u]];
            } else {
                r.push_back({chain_id[v], chain_pos[head[v]], chain_pos[v], 0});
                v = prev[head[v]];
            }

        if (chain_pos[u] <= chain_pos[v]) l.push_back({chain_id[u], chain_pos[u], chain_pos[v], 0});
        else l.push_back({chain_id[u], chain_pos[v], chain_pos[u], 1});

        for (int i = r.size() - 1; ~i; i--) l.emplace_back(r[i]);
        return l;
    };

    vector<vector<pair<int, int>>> edges(lg_len + 1);
    while (Q--) {
        int u, v, x, y;
        cin >> u >> v >> x >> y;

        if (dist(u, v) == dist(x, y)) {
            auto path1 = decompose(u, v), path2 = decompose(x, y);
            for (int i = 0, j = 0, o1 = 0, o2 = 0; i < path1.size() && j < path2.size();) {
                auto [c1, l1, r1, dir1] = path1[i];
                auto [c2, l2, r2, dir2] = path2[j];
                int len1 = r1 - l1 + 1 - o1, len2 = r2 - l2 + 1 - o2, len = min(len1, len2);

                if (!dir1) l1 += o1;
                else l1 = r1 - o1 - len + 1;
                r1 = l1 + len - 1;
                o1 += len;

                if (!dir2) l2 += o2;
                else l2 = r2 - o2 - len + 1;
                r2 = l2 + len - 1;
                o2 += len;

                while (len) {
                    int l = __lg(len), s = 1 << l,
                        p1 = dir1 ? r1 - s + 1 : l1,
                        p2 = dir2 ? r2 - s + 1 : l2;

                    edges[l].emplace_back(encode(l, c1, p1, dir1), encode(l, c2, p2, dir2));

                    if (dir1) r1 -= s;
                    else l1 += s;
                    if (dir2) r2 -= s;
                    else l2 += s;

                    len -= s;
                }

                if (o1 == path1[i][2] - path1[i][1] + 1) {
                    o1 = 0;
                    i++;
                }

                if (o2 == path2[j][2] - path2[j][1] + 1) {
                    o2 = 0;
                    j++;
                }
            }
        }
    }

    auto build = [&](int l) {
        vector<int> indices(pref[l].back(), -1), order;
        for (auto [u, v] : edges[l]) {
            if (!~indices[u]) {
                order.emplace_back(u);
                indices[u] = order.size() - 1;
            }

            if (!~indices[v]) {
                order.emplace_back(v);
                indices[v] = order.size() - 1;
            }
        }

        int m = order.size();
        DisjointSets dsu(m);
        for (auto [u, v] : edges[l]) dsu.unite(indices[u], indices[v]);

        vector<vector<int>> groups(m);
        for (int i = 0; i < m; i++) groups[dsu.find(i)].emplace_back(order[i]);
        return groups;
    };

    for (int l = lg_len; l; l--) {
        if (edges[l].empty()) continue;

        int s = 1 << (l - 1);
        for (auto &g : build(l))
            if (g.size() > 1) {
                auto [c0, p0, dir0] = decode(l, g[0]);
                int ul = encode(l - 1, c0, p0 + dir0 * s, dir0), ur = encode(l - 1, c0, p0 + !dir0 * s, dir0);
                for (int i = 1; i < g.size(); i++) {
                    auto [c1, p1, dir1] = decode(l, g[i]);
                    int vl = encode(l - 1, c1, p1 + dir1 * s, dir1), vr = encode(l - 1, c1, p1 + !dir1 * s, dir1);
                    edges[l - 1].emplace_back(ul, vl);
                    edges[l - 1].emplace_back(ur, vr);
                }
            }
    }

    DisjointSets dsu(n + 1);
    for (auto &g : build(0))
        if (g.size() > 1) {
            auto [c0, p0, _] = decode(0, g[0]);
            int u = chains[c0][p0];
            for (int i = 1; i < g.size(); i++) {
                auto [c1, p1, _] = decode(0, g[i]);
                int v = chains[c1][p1];
                dsu.unite(u, v);
            }
        }

    vector<vector<int>> groups(n + 1);
    for (int i = 1; i <= n; i++) groups[dsu.find(i)].emplace_back(a[i]);

    auto normalize = [&](int x) {
        return (x + k) % k;
    };

    auto time = 0LL;
    for (auto &g : groups) {
        if (g.empty()) continue;

        sort(g.begin(), g.end());
        if (!k) {
            for (int ai : g) time += abs(ai - g[g.size() / 2]);
            continue;
        }

        if (k == 1) {
            int same = 1, longest = 1;
            for (int i = 1; i < g.size(); i++)
                if (g[i] == g[i - 1]) same++;
                else {
                    longest = max(longest, same);
                    same = 1;
                }
            time += g.size() - max(longest, same);
            continue;
        }

        unordered_map<int, pair<int, int>> rems;
        for (int i = 0, j = 1; i < g.size(); i = j++) {
            for (; j < g.size() && g[i] == g[j]; j++);
            auto &[freq, len] = rems[normalize(g[i])];
            freq += j - i;
            len = max(len, j - i);
        }

        vector<int> distinct;
        for (auto [rem, p] : rems) distinct.emplace_back(rem);
        sort(distinct.begin(), distinct.end());
        int s1 = distinct.size(), s2 = 2 * s1;

        vector<int> cycle(s2), freqCyc(s2);
        for (int i = 0; i < s1; i++) {
            cycle[i] = distinct[i];
            cycle[i + s1] = distinct[i] + k;
        }

        vector<long long> pref1(s2 + 1, 0), pref2(s2 + 1, 0);
        for (int i = 0; i < s2; i++) {
            int freq = rems[distinct[i % s1]].first;
            pref1[i + 1] = pref1[i] + freq;
            pref2[i + 1] = pref2[i] + (long long) freq * cycle[i];
        }

        int pivot = k / 2;
        vector<int> temp;
        for (int rem : distinct) {
            temp.emplace_back(rem);
            temp.emplace_back(normalize(rem + pivot));
            temp.emplace_back(normalize(rem - pivot));
            if (k & 1) {
                temp.emplace_back(normalize(rem + pivot + 1));
                temp.emplace_back(normalize(rem - pivot - 1));
            }
        }
        sort(temp.begin(), temp.end());
        temp.erase(unique(temp.begin(), temp.end()), temp.end());

        vector<int> all;
        for (int r : temp) {
            all.emplace_back(normalize(r - 1));
            all.emplace_back(r);
            all.emplace_back(normalize(r + 1));
        }
        sort(all.begin(), all.end());
        all.erase(unique(all.begin(), all.end()), all.end());

        auto t = LLONG_MAX;
        for (int rem : all) {
            int p_0 = upper_bound(cycle.begin(), cycle.end(), rem) - cycle.begin(),
                p_p = upper_bound(cycle.begin(), cycle.end(), rem + pivot) - cycle.begin(),
                p_k = upper_bound(cycle.begin(), cycle.end(), rem + k - 1) - cycle.begin();

            auto f1 = pref1[p_p] - pref1[p_0], sum1 = pref2[p_p] - pref2[p_0],
                 f2 = pref1[p_k] - pref1[p_p], sum2 = pref2[p_k] - pref2[p_p],
                 total = (sum1 - f1 * rem) + f2 * (rem + k) - sum2;

            auto it = rems.find(rem);
            if (it != rems.end()) {
                auto [freq, len] = it->second;
                total += 2 * (freq - len);
            }
            t = min(t, total);
        }

        time += t;
    }
    cout << time;
}
