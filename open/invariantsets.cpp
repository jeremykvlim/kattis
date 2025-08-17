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

ostream & operator<<(ostream &stream, const __int128 &v) {
    if (abs(v) <= LLONG_MAX) return stream << (long long) v;

    string s;
    auto temp = v;
    while (temp) {
        s += '0' + (temp % 10);
        temp /= 10;
    }
    if (v < 0) s += '-';
    reverse(s.begin(), s.end());
    return stream << s;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m;
    cin >> m;

    auto read = []() -> vector<int> {
        int degree;
        cin >> degree;

        vector<int> poly(degree + 1);
        for (int &a : poly) cin >> a;

        return poly;
    };
    auto p = read(), q = read();

    vector<int> f_p(m), f_q(m);
    for (int x = 0; x < m; x++) {
        auto eval = [&](const auto &poly) {
            int y = 0;
            for (int a : poly) y = (y * x + a) % m;
            return y;
        };

        f_p[x] = eval(p);
        f_q[x] = eval(q);
    }

    auto find_cycles = [&](const auto &f) -> tuple<vector<bool>, vector<int>, vector<vector<int>>> {
        int n = f.size();
        vector<int> degree(n, 0);
        for (int y : f) degree[y]++;

        queue<int> q;
        for (int v = 0; v < n; v++)
            if (!degree[v]) q.emplace(v);

        vector<bool> visited(n, false);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            if (visited[v]) continue;
            visited[v] = true;

            int u = f[v];
            if (!--degree[u]) q.emplace(u);
        }

        vector<int> cycle_id(n, -1);
        vector<bool> seen(n, false);
        vector<vector<int>> cycles;
        for (int v = 0; v < n; v++)
            if (!visited[v] && !seen[v]) {
                vector<int> cycle;
                int u = v;
                do {
                    cycle.emplace_back(u);
                    seen[u] = true;
                    u = f[u];
                } while (u != v);
                cycles.emplace_back(cycle);
                for (int t : cycle) cycle_id[t] = cycles.size() - 1;
            }

        return {visited, cycle_id, cycles};
    };
    auto [visited_p, cycle_id_p, cycles_p] = find_cycles(f_p);
    auto [visited_q, cycle_id_q, cycles_q] = find_cycles(f_q);

    DisjointSets dsu(m);
    vector<bool> valid_p(cycles_p.size(), true), valid_q(cycles_q.size(), true);
    auto check = [&](const auto &cycles, auto &valid1, auto &visited2) {
        for (int i = 0; i < cycles.size(); i++) {
            for (int v : cycles[i])
                if (visited2[v]) {
                    valid1[i] = false;
                    goto next;
                }

            for (int k = 1; k < cycles[i].size(); k++) dsu.unite(cycles[i][0], cycles[i][k]);
            next:;
        }
    };
    check(cycles_p, valid_p, visited_q);
    check(cycles_q, valid_q, visited_p);

    __int128 mask = 0;
    for (int v = 0; v < m; v++) {
        int i = cycle_id_p[v], j = cycle_id_q[v];
        if (i != -1 && j != -1 && valid_p[i] && valid_q[j]) mask |= ((__int128) 1) << dsu.find(v);
    }
    cout << (((__int128) 1) << popcount((unsigned __int128) mask));
}
