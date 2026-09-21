#include <bits/stdc++.h>
using namespace std;

inline char readchar() {
    static const int size = 1 << 20;
    static array<char, size> buf;
    static int pos = 0, len = 0;
    if (pos >= len) {
        pos = 0;
        len = fread(buf.data(), 1, size, stdin);
        if (!len) return EOF;
    }
    return buf[pos++];
}

template <typename T>
inline bool read(T &v) {
    char c;
    do {
        c = readchar();
        if (c == EOF) return false;
    } while (c == ' ' || c == '\n' || c == '\r' || c == '\t');

    if constexpr (is_integral_v<T> && !is_same_v<T, char>) {
        bool neg = false;
        if (c == '+' || c == '-') {
            neg = (c == '-');
            c = readchar();
        }

        v = 0;
        for (; '0' <= c && c <= '9'; c = readchar()) v = v * 10 + (c - '0');
        if (neg) v = -v;
        return true;
    } else if constexpr (is_floating_point_v<T>) {
        bool neg = false;
        if (c == '+' || c == '-') {
            neg = (c == '-');
            c = readchar();
        }

        v = 0;
        for (; '0' <= c && c <= '9'; c = readchar()) v = v * 10 + (c - '0');
        if (c == '.') {
            T place = 1;
            for (c = readchar(); '0' <= c && c <= '9'; c = readchar()) {
                place *= 0.1;
                v += (c - '0') * place;
            }
        }
        if (neg) v = -v;
        return true;
    } else if constexpr (is_same_v<T, char>) {
        v = c;
        return true;
    } else if constexpr (is_same_v<T, string>) {
        v.clear();
        do {
            v += c;
            c = readchar();
        } while (c != EOF && c != ' ' && c != '\n' && c != '\r' && c != '\t');
        return true;
    }

    return false;
}

template <typename... T>
inline bool read(T &... xs) requires (sizeof...(T) > 1) {
    return (read(xs) && ...);
}

struct RollbackDisjointSets {
    vector<int> sets;
    vector<pair<int, int>> history;

    int find(int v) {
        while (sets[v] >= 0) v = sets[v];
        return v;
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return false;

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        history.emplace_back(v_set, sets[v_set]);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        return true;
    }

    int size(int v) {
        return -sets[find(v)];
    }

    int record() {
        return history.size();
    }

    void rollback(int version) {
        while (record() > version) {
            auto [v_set, s] = history.back();
            history.pop_back();

            int u_set = sets[v_set];
            sets[u_set] -= s;
            sets[v_set] = s;
        }
    }

    void delete_history(int version = 0) {
        history.resize(version);
    }

    RollbackDisjointSets(int n) : sets(n, -1) {}
};

template <typename T>
pair<T, vector<int>> edmonds_dense(int n, vector<vector<T>> &adj_matrix_transpose, int root = 0) {
    vector<vector<pair<int, int>>> edge(n, vector<pair<int, int>>(n, {-1, -1}));
    for (int u = 0; u < n; u++)
        for (int v = 0; v < n; v++) edge[u][v] = {v, u};

    RollbackDisjointSets rdsu(n);
    vector<pair<int, int>> chosen_edge(n, {-1, -1});
    stack<tuple<int, int, vector<pair<int, int>>>> history;
    vector<bool> cycle(n, false);
    vector<int> cycle_nodes, rep(n), active(n), seen(n, -1);
    iota(rep.begin(), rep.end(), 0);
    iota(active.begin(), active.end(), 0);
    seen[root] = root;
    T len = 0;
    for (int s = 0; s < n; s++) {
        int v = rep[rdsu.find(s)];
        while (!~seen[v]) {
            T w = numeric_limits<T>::max();
            for (int u : active)
                if (w > adj_matrix_transpose[v][u]) {
                    w = adj_matrix_transpose[v][u];
                    chosen_edge[v] = edge[v][u];
                }
            if (w == numeric_limits<T>::max()) return {0, {}};

            seen[v] = s;
            len += w;
            for (int u : active)
                if (u != v) adj_matrix_transpose[v][u] -= w;

            int r = rep[rdsu.find(chosen_edge[v].first)];
            if (seen[r] != s) {
                v = r;
                continue;
            }

            cycle[r] = true;
            cycle_nodes.emplace_back(r);
            for (int u = rep[rdsu.find(chosen_edge[r].first)]; u != r; u = rep[rdsu.find(chosen_edge[u].first)]) {
                cycle[u] = true;
                cycle_nodes.emplace_back(u);
            }

            for (int u : active)
                if (!cycle[u])
                    for (int i = 1; i < cycle_nodes.size(); i++) {
                        int t = cycle_nodes[i];
                        if (adj_matrix_transpose[r][u] > adj_matrix_transpose[t][u]) {
                            adj_matrix_transpose[r][u] = adj_matrix_transpose[t][u];
                            edge[r][u] = edge[t][u];
                        }
                        if (adj_matrix_transpose[u][r] > adj_matrix_transpose[u][t]) {
                            adj_matrix_transpose[u][r] = adj_matrix_transpose[u][t];
                            edge[u][r] = edge[u][t];
                        }
                    }

            int version = rdsu.record();
            vector<pair<int, int>> cycle_edges;
            for (int u : cycle_nodes) cycle_edges.emplace_back(chosen_edge[u]);
            history.emplace(version, r, cycle_edges);
            for (int i = 1; i < cycle_nodes.size(); i++) rdsu.unite(r, cycle_nodes[i]);
            rep[rdsu.find(r)] = r;

            active.erase(remove_if(active.begin(), active.end(), [&](int u) { return cycle[u] && u != r; }), active.end());
            while (!cycle_nodes.empty()) {
                cycle[cycle_nodes.back()] = false;
                cycle_nodes.pop_back();
            }
            chosen_edge[r] = {-1, -1};
            seen[r] = -1;
            v = r;
        }
    }

    vector<pair<int, int>> edges(n, {-1, -1});
    for (int v : active)
        if (v != root) edges[rdsu.find(chosen_edge[v].second)] = chosen_edge[v];

    while (!history.empty()) {
        auto [version, r, cycle_edges] = history.top();
        history.pop();

        auto e = edges[rdsu.find(r)];
        rdsu.rollback(version);
        for (auto f : cycle_edges) edges[rdsu.find(f.second)] = f;
        edges[rdsu.find(e.second)] = e;
    }

    vector<int> mst(n);
    for (int v = 0; v < n; v++) mst[v] = edges[v].first;
    mst[root] = root;
    return {len, mst};
}

int main() {
    ios::sync_with_stdio(false);

    int n;
    read(n);

    vector<vector<long long>> adj_matrix_transpose(n + 1, vector<long long>(n + 1, LLONG_MAX));
    for (int i = 1; i <= n; i++) {
        int x, s;
        read(x, s);

        for (int j = 0; j <= n; j++) {
            int a;
            read(a);

            if (i != j) adj_matrix_transpose[i][j] = j == x ? min(a, s) : a;
        }
    }
    cout << edmonds_dense(n + 1, adj_matrix_transpose).first;
}