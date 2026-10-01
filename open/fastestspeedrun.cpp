#include <bits/stdc++.h>
using namespace std;

struct Scanner {
    static constexpr int size = 1 << 20;
    array<char, size + 1> buf{};
    int pos = 0, len = 0;
    bool valid = true;

    inline bool reload() {
        pos = 0;
        len = fread(buf.data(), 1, size, stdin);
        buf[len] = 0;
        return len;
    }

    inline bool skip_space() {
        for (;;) {
            for (; buf[pos] == ' ' || buf[pos] == '\n' || buf[pos] == '\r' || buf[pos] == '\t'; pos++);
            if (pos < len) return true;
            if (!reload()) return false;
        }
    }

    template <typename T>
    inline bool read(T &v) {
        if (!skip_space()) return false;

        if constexpr (is_integral_v<T> && !is_same_v<T, char>) {
            bool neg = false;
            if (buf[pos] == '+' || buf[pos] == '-') {
                neg = buf[pos] == '-';
                if (++pos == len && !reload()) return false;
            }

            v = 0;
            for (;;) {
                for (; '0' <= buf[pos] && buf[pos] <= '9'; pos++) v = v * 10 + (buf[pos] - '0');
                if (pos < len || !reload()) break;
            }
            if (neg) v = -v;
            return true;
        } else if constexpr (is_floating_point_v<T>) {
            bool neg = false;
            if (buf[pos] == '+' || buf[pos] == '-') {
                neg = buf[pos] == '-';
                if (++pos == len && !reload()) return false;
            }

            v = 0;
            for (;;) {
                for (; '0' <= buf[pos] && buf[pos] <= '9'; pos++) v = v * 10 + (buf[pos] - '0');
                if (pos < len || !reload()) break;
            }
            if (buf[pos] == '.') {
                if (++pos == len) reload();
                T place = 1;
                for (;;) {
                    for (; '0' <= buf[pos] && buf[pos] <= '9'; pos++) {
                        place *= (T) 0.1;
                        v += (buf[pos] - '0') * place;
                    }
                    if (pos < len || !reload()) break;
                }
            }
            if (neg) v = -v;
            return true;
        } else if constexpr (is_same_v<T, char>) {
            v = buf[pos++];
            return true;
        } else if constexpr (is_same_v<T, string>) {
            v.clear();
            for (;;) {
                int prev = pos;
                for (; buf[pos] && buf[pos] != ' ' && buf[pos] != '\n' && buf[pos] != '\r' && buf[pos] != '\t'; pos++);
                v.append(buf.begin() + prev, buf.begin() + pos);
                if (pos < len || !reload()) break;
            }
            return true;
        }

        return false;
    }

    template <typename T>
    inline bool read(vector<T> &v) {
        for (auto &x : v)
            if (!read(x)) return false;
        return true;
    }

    template <typename T, typename U>
    inline bool read(pair<T, U> &p) {
        return read(p.first) && read(p.second);
    }

    template <typename... T>
    inline bool read(tuple<T...> &t) {
        return apply([&](auto &...x) { return (read(x) && ...); }, t);
    }

    template <typename... T>
    inline bool read(T &...x) requires (sizeof...(T) > 1) {
        return (read(x) && ...);
    }

    template <typename T>
    requires (!requires(Scanner &s, T &v) { operator>>(s, v); })
    inline Scanner & operator>>(T &v) {
        if (valid) valid = read(v);
        return *this;
    }

    explicit operator bool() const {
        return valid;
    }
};

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
    Scanner scan;

    int n;
    scan >> n;

    vector<vector<long long>> adj_matrix_transpose(n + 1, vector<long long>(n + 1, LLONG_MAX));
    for (int i = 1; i <= n; i++) {
        int x, s;
        scan >> x >> s;

        for (int j = 0; j <= n; j++) {
            int a;
            scan >> a;

            if (i != j) adj_matrix_transpose[i][j] = j == x ? min(a, s) : a;
        }
    }
    cout << edmonds_dense(n + 1, adj_matrix_transpose).first;
}