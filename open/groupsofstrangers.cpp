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

template <typename T>
T gospers_hack(T mask) {
    T lsb_mask = mask | (mask - 1);
    return (lsb_mask + 1) | (((~lsb_mask & -~lsb_mask) - 1) >> (countr_zero(mask) + 1));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> degree(n, 0);
    vector<vector<int>> adj_list(n);
    vector<vector<bool>> adj_matrix(n, vector<bool>(n, false));
    vector<pair<int, int>> edges(m);
    for (auto &[a, b] : edges) {
        cin >> a >> b;
        a--;
        b--;

        degree[a]++;
        degree[b]++;
        adj_list[a].emplace_back(b);
        adj_list[b].emplace_back(a);
        adj_matrix[a][b] = adj_matrix[b][a] = true;
    }

    for (int s = 0; s < n; s++)
        if (degree[s] && degree[s] <= 15) {
            queue<int> q;
            q.emplace(s);
            vector<int> dist(n, -1);
            dist[s] = 0;
            while (!q.empty()) {
                int v = q.front();
                q.pop();

                if (dist[v] == 2) continue;

                for (int u : adj_list[v])
                    if (dist[u] == -1) {
                        dist[u] = dist[v] + 1;
                        q.emplace(u);
                    }
            }
            bool valid = true;
            for (int i = 0; i < n; i++)
                if (dist[i] == -1 || dist[i] > 2) {
                    valid = false;
                    break;
                }
            if (!valid) continue;

            int d = degree[s];
            vector<bitset<1000>> coverage(d, 0);
            for (int i = 0; i < d; i++) {
                int v = adj_list[s][i];
                coverage[i][v] = true;
                for (int u : adj_list[v]) coverage[i][u] = true;
            }

            bitset<1000> target(0);
            for (int i = 0; i < n; i++)
                if (i != s) target.set(i);

            vector<int> neighbors;
            auto check_subsets = [&](int k) {
                for (auto m1 = (1U << k) - 1; m1 < 1 << d;) {
                    bitset<1000> temp;
                    for (auto i = m1; i; i &= i - 1) temp |= coverage[countr_zero(i)];
                    if ((temp & target) == target) {
                        for (int i = 0; i < d; i++)
                            if ((m1 >> i) & 1) neighbors.emplace_back(adj_list[s][i]);
                        return true;
                    }
                    auto m2 = gospers_hack(m1);
                    if (m1 >= m2) break;
                    m1 = m2;
                }
                return false;
            };

            if (d <= 8) {
                for (int k = 1; k <= d; k++)
                    if (check_subsets(k)) break;
            } else check_subsets(8);
            if (neighbors.empty()) continue;

            int k = neighbors.size();
            vector<int> indices(n, -1);
            for (int i = 0; i < k; i++) indices[neighbors[i]] = i;

            for (int assign = 0; assign < 1 << k; assign++) {
                vector<unsigned> colors(n, (1 << 3) - 1);
                q = queue<int>();
                auto propagate = [&](int v, int bit) -> bool {
                    if (!has_single_bit(colors[v])) colors[v] &= bit;
                    if (!colors[v]) return false;
                    if (has_single_bit(colors[v])) q.emplace(v);
                    return true;
                };
                if (!propagate(s, 1 << 2)) continue;

                valid = true;
                for (int i = 0; i < k; i++)
                    if (!propagate(neighbors[i], 1 << ((assign >> i) & 1))) {
                        valid = false;
                        break;
                    }
                if (!valid) continue;

                for (int i = 0; i < k && valid; i++)
                    for (int j = i + 1; j < k; j++)
                        if (adj_matrix[neighbors[i]][neighbors[j]] && colors[neighbors[i]] == colors[neighbors[j]]) {
                            valid = false;
                            break;
                        }
                if (!valid) continue;

                while (!q.empty() && valid) {
                    int v = q.front();
                    q.pop();

                    for (int u : adj_list[v])
                        if (colors[u] & colors[v]) {
                            colors[u] &= ~colors[v];
                            if (!colors[u]) {
                                valid = false;
                                break;
                            }
                            if (has_single_bit(colors[u])) q.emplace(u);
                        }
                }
                if (!valid) continue;

                for (int v = 0; v < n; v++)
                    if (v != s && popcount(colors[v]) == 3) {
                        valid = false;
                        break;
                    }
                if (!valid) continue;

                int count = 0;
                vector<int> id(n, -1);
                vector<array<int, 2>> options(n, {-1, -1});
                for (int v = 0; v < n; v++)
                    if (!has_single_bit(colors[v])) {
                        int c1 = countr_zero(colors[v]), c2 = countr_zero(colors[v] ^ (1 << c1));
                        options[v] = {c1, c2};
                        id[v] = count++;
                    }

                TwoSATSystem sat(count);
                auto ban_color = [&](int v, int c) {
                    if (id[v] == -1) return;

                    auto [c1, c2] = options[v];
                    if (c1 == c && c2 != c) sat.assign(id[v]);
                    else if (c2 == c && c1 != c) sat.assign(~id[v]);
                };

                for (auto [u, v] : edges) {
                    if (has_single_bit(colors[u]) && has_single_bit(colors[v])) {
                        if (colors[u] == colors[v]) {
                            valid = false;
                            break;
                        }
                        continue;
                    }

                    if (!has_single_bit(colors[v]) && has_single_bit(colors[u])) {
                        ban_color(v, countr_zero(colors[u]));
                        continue;
                    }

                    if (!has_single_bit(colors[u]) && has_single_bit(colors[v])) {
                        ban_color(u, countr_zero(colors[v]));
                        continue;
                    }

                    if (!has_single_bit(colors[u]) && !has_single_bit(colors[v]))
                        for (int i = 0; i < 2; i++)
                            for (int j = 0; j < 2; j++)
                                if (options[u][i] == options[v][j]) sat.add_clause(!i ? id[u] : ~id[u], !j ? id[v] : ~id[v]);
                }
                if (!valid) continue;

                auto [satisfiable, assignment] = sat.solve();
                if (!satisfiable) continue;

                for (int v = 0; v < n; v++)
                    if (has_single_bit(colors[v])) cout << countr_zero(colors[v]) + 1 << " ";
                    else cout << options[v][assignment[id[v]]] + 1 << " ";
                exit(0);
            }
        }
    cout << "Impossible";
}
