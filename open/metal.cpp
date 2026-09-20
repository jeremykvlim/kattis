#include <bits/stdc++.h>
using namespace std;

struct TwoSATSystem {
    int n;
    vector<vector<int>> adj_list;

    TwoSATSystem(int n = 0) : n(n), adj_list(2 * n) {}

    int add_variable() {
        adj_list.emplace_back();
        adj_list.emplace_back();
        return n++;
    }

    void add_clause(int i, int j) {
        i = max(i << 1, -(i << 1 | 1));
        j = max(j << 1, -(j << 1 | 1));
        adj_list[i ^ 1].emplace_back(j);
        adj_list[j ^ 1].emplace_back(i);
    }

    void assign(int i) {
        add_clause(i, i);
    }

    void add_at_most_one(const vector<int> &literals) {
        if (literals.size() < 2) return;

        int prev = ~literals[0];
        for (int i = 2; i < literals.size(); i++) {
            int curr = add_variable();
            add_clause(prev, ~literals[i]);
            add_clause(prev, curr);
            add_clause(~literals[i], curr);
            prev = ~curr;
        }
        add_clause(prev, ~literals[1]);
    }

    pair<bool, vector<int>> solve() {
        deque<int> q;
        vector<int> assignment(n, -1);
        auto bfs = [&](int s) {
            q = {s};
            assignment[s >> 1] = !(s & 1);
            for (int i = 0; i < q.size(); i++) {
                int v = q[i];
                for (int u : adj_list[v])
                    if (assignment[u >> 1] == -1) {
                        assignment[u >> 1] = !(u & 1);
                        q.emplace_back(u);
                    } else if (assignment[u >> 1] == (u & 1)) return false;
            }
            return true;
        };

        for (int i = 0; i < n; i++) {
            if (assignment[i] != -1 || bfs(i << 1 | 1)) continue;
            for (int v : q) assignment[v >> 1] = -1;
            if (!bfs(i << 1)) return {false, {}};
        }
        return {true, assignment};
    }
};

struct DisjointSets {
    vector<int> sets, parity;

    pair<int, int> find(int v) {
        int flip = 0;
        while (sets[v] >= 0) {
            flip ^= parity[v];
            int p = sets[v];
            if (sets[p] >= 0) {
                parity[v] ^= parity[p];
                sets[v] = sets[p];
            }
            v = p;
        }
        return {v, flip};
    }

    int unite(int u, int v, int p) {
        auto [u_set, u_parity] = find(u);
        auto [v_set, v_parity] = find(v);
        if (u_set == v_set) return (u_parity ^ v_parity) == p ? 0 : -1;

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        parity[v_set] = u_parity ^ v_parity ^ p;
        return 1;
    }

    int size(int v) {
        return -sets[find(v).first];
    }

    DisjointSets(int n) : sets(n, -1), parity(n, 0) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> dists{0};
    vector<array<int, 3>> edges;
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++) {
            int d;
            cin >> d;

            dists.emplace_back(d);
            edges.push_back({d, i, j});
        }
    sort(dists.begin(), dists.end());
    dists.erase(unique(dists.begin(), dists.end()), dists.end());
    sort(edges.rbegin(), edges.rend());

    DisjointSets dsu(n);
    vector<int> candidates;
    for (int l = 0, r = 1; l < edges.size() && edges[l][0]; l = r++) {
        int d = edges[l][0];
        for (; r < edges.size() && edges[r][0] == d; r++);

        bool changed = false;
        for (int k = l; k < r; k++) {
            auto [_, u, v] = edges[k];
            int state = dsu.unite(u, v, 1);
            if (!~state) {
                candidates.emplace_back(d);
                goto done;
            }
            changed |= state;
        }
        if (changed) candidates.emplace_back(d);
    }
    candidates.emplace_back(0);

    done:;
    TwoSATSystem sat(n);
    int sum = INT_MAX;
    for (int l = -1; int d : candidates) {
        int r = lower_bound(dists.begin(), dists.end(), d) - dists.begin(), m;
        while (l + 1 < r) {
            m = l + (r - l) / 2;

            auto feasible = [&]() {
                for (auto &neighbors : sat.adj_list) neighbors.clear();

                for (auto [w, u, v] : edges) {
                    if (w <= dists[m]) break;
                    sat.add_clause(~u, ~v);
                    if (w > d) sat.add_clause(u, v);
                }
                return sat.solve().first;
            };

            if (feasible()) r = m;
            else l = m;
        }
        l = r - 1;
        sum = min(sum, dists[r] + d);
    }
    cout << sum;
}
