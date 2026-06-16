#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets;

    int find(int v) {
        while (sets[v] >= 0) {
            int p = sets[v];
            if (sets[p] >= 0) sets[v] = sets[p];
            v = p;
        }
        return v;
    }

    pair<int, int> unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return {u_set, -1};

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        return {u_set, v_set};
    }

    int size(int v) {
        return -sets[find(v)];
    }

    DisjointSets(int n) : sets(n, -1) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> d(n + 1);
    vector<double> c(n + 1), p(n + 1), ratio(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> c[i] >> p[i] >> d[i];

        ratio[i] = c[i] / (1 - p[i]);
    }

    DisjointSets dsu(n + 1);
    vector<int> rep(n + 1);
    iota(rep.begin(), rep.end(), 0);
    vector<list<int>> seq(n + 1);
    for (int i = 1; i <= n; i++) seq[i].emplace_back(i);

    vector<bool> visited(n + 1, false);
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;
    for (int i = 1; i <= n; i++) pq.emplace(ratio[i], i);
    while (!pq.empty()) {
        auto [ai, i] = pq.top();
        pq.pop();

        if (visited[i] || rep[dsu.find(i)] != i || ai != ratio[i]) continue;
        visited[i] = true;

        int d_set = rep[dsu.find(d[i])];
        auto [big, small] = dsu.unite(d_set, i);
        if (small != -1) {
            rep[big] = d_set;

            seq[d_set].splice(seq[d_set].end(), seq[i]);
            c[d_set] += p[d_set] * c[i];
            p[d_set] *= p[i];
            ratio[d_set] = c[d_set] / (1 - p[d_set]);

            pq.emplace(ratio[d_set], d_set);
        }
    }

    for (int i : seq[0]) cout << i << "\n";
}