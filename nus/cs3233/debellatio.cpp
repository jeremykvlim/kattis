#include <bits/stdc++.h>
using namespace std;

struct DisjointSet {
    vector<int> sets;

    int find(int p) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p]));
    }

    bool unite(int p, int q) {
        int p_set = find(p), q_set = find(q);
        if (p_set != q_set) {
            sets[q_set] = p_set;
            return true;
        }
        return false;
    }

    DisjointSet(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

pair<int, vector<int>> micali_vazirani(int n, vector<pair<int, int>> edges) {
    vector<int> match(n, -1);
    vector<vector<int>> adj_list(n);
    for (auto [u, v] : edges) {
        adj_list[u].emplace_back(v);
        adj_list[v].emplace_back(u);
    }

    vector<int> visited(n, -1);
    int t = -1;
    bool change;
    do {
        change = false;
        DisjointSet dsu(n);
        vector<array<int, 2>> parent(n, {-1, -1});
        vector<array<int, 3>> fw(n, {-1, -1, -1}), bw(n, {-1, -1, -1});
        vector<int> order(n), anc(n), dir(n, -1), level(n, -1), depth(n, -1), lift(n, -1), state(n, -1);
        iota(order.begin(), order.end(), 0);
        iota(anc.begin(), anc.end(), 0);
        int count = 0;

        stack<int> s;
        for (int i = 0; i < n; i++)
            if (match[i] == -1) {
                state[i] = level[i] = 0;
                s.emplace(i);
            }

        while (!s.empty() && !change) {
            int v = s.top();
            s.pop();

            for (int u : adj_list[v]) {
                int p = dsu.find(v), q = dsu.find(u);
                if (p == q || state[q] == 1) continue;

                if (state[q] == -1) {
                    int m = match[q];
                    state[q] = 1;
                    state[m] = 0;
                    parent[q] = {v, u};
                    parent[m] = {u, m};
                    s.emplace(m);
                } else {
                    auto lca = [&]() {
                        t++;
                        int p_set = dsu.find(p), q_set = dsu.find(q);
                        while (p_set >= 0 || q_set >= 0) {
                            if (p_set != -1) {
                                if (visited[p_set] == t) return p_set;
                                visited[p_set] = t;
                                p_set = parent[p_set][0] >= 0 ? dsu.find(parent[p_set][0]) : parent[p_set][0];
                            }
                            swap(p_set, q_set);
                        }
                        return -1;
                    };

                    int a = lca();
                    if (a != -1) {
                        level[a] = ++count;
                        order.emplace_back(a);

                        auto dual_traversal = [&](bool rev) {
                            int e = !rev ? v : u, f = !rev ? u : v;
                            for (int e_set = dsu.find(e); e_set != a;) {
                                int f_set = dsu.find(f), g = parent[e_set][0], g_set = dsu.find(g), h = parent[e_set][1];

                                if (state[e_set] == 1) s.emplace(e_set);

                                dir[e_set] = state[e_set] ^ rev;
                                anc[e_set] = a;
                                fw[e_set] = {g_set, g, h};
                                bw[e_set] = {f_set, f, e};

                                if (rev) swap(fw[e_set], bw[e_set]);

                                e = g;
                                f = h;
                                e_set = g_set;
                            }
                        };
                        dual_traversal(false);
                        dual_traversal(true);

                        auto contract = [&](int r) {
                            for (int b = r; b != a; b = dsu.find(parent[b][0])) dsu.unite(a, b);
                        };
                        contract(p);
                        contract(q);
                    } else {
                        for (int i = order.size() - 1; ~i; i--) {
                            int r = order[i];
                            if (depth[r] == -1) {
                                a = anc[r];
                                depth[r] = depth[a] + 1;
                                lift[r] = a;
                                if (depth[r] > 1 && depth[a] - depth[lift[a]] == depth[lift[a]] - depth[lift[lift[a]]]) lift[r] = lift[lift[a]];
                            }
                        }

                        deque<pair<int, int>> path;
                        for (int r = p; parent[r][0] != -1; r = dsu.find(parent[r][0])) path.emplace_front(parent[r][0], parent[r][1]);
                        path.emplace_front(-1, parent[p][0] == -1 ? p : dsu.find(path.front().first));
                        path.emplace_back(v, u);
                        for (int r = q; parent[r][0] != -1; r = dsu.find(parent[r][0])) path.emplace_back(parent[r][1], parent[r][0]);
                        path.emplace_back(parent[q][0] == -1 ? q : dsu.find(path.back().second), -1);

                        for (auto it = next(path.begin()); it != path.end();) {
                            auto [e, f] = *prev(it);
                            auto [g, h] = *it;

                            if (f == g) {
                                it++;
                                continue;
                            }

                            bool rev = h == -1 || match[g] == h;
                            if (rev) {
                                swap(e, h);
                                swap(f, g);
                            }
                            while (level[anc[g]] < level[f]) g = level[lift[g]] < level[f] ? lift[g] : anc[g];

                            deque<pair<int, int>> temp;
                            auto add = [&](auto d) {
                                for (int i = g; i != f; i = d[i][0]) temp.emplace_front(d[i][1], d[i][2]);
                            };
                            add(!dir[g] ? fw : bw);

                            if (!rev) it = path.insert(it, temp.begin(), temp.end());
                            else {
                                for (auto &[x, y] : temp) swap(x, y);
                                it = path.insert(it, temp.rbegin(), temp.rend());
                            }
                        }
                        path.pop_front();
                        path.pop_back();

                        for (auto it = path.begin();; it = next(it, 2)) {
                            match[it->first] = it->second;
                            match[it->second] = it->first;
                            if (next(it) == path.end()) break;
                        }
                        change = true;
                    }
                }
                if (change) break;
            }
        }
    } while (change);

    int matches = (n - count(match.begin(), match.end(), -1)) / 2;
    return {matches, match};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<pair<int, int>> edges;
    while (m--) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;

        edges.emplace_back(a, b);
    }

    auto [matches, match] = micali_vazirani(n, edges);
    if (matches * 2 == n) cout << "2\n" << flush;
    else {
        cout << "1\n" << flush;
        for (int a = 0; a < n; a++)
            if (match[a] == -1) {
                cout << a + 1 << "\n" << flush;
                break;
            }
    }

    for (;;) {
        int a;
        cin >> a;

        if (!a) exit(0);

        cout << match[a - 1] + 1 << "\n" << flush;
    }
}
