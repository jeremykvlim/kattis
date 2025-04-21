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

    int n;
    cin >> n;

    vector<int> c(n + 1), d(n + 1);
    vector<double> p(n + 1), ratio(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> c[i] >> p[i] >> d[i];

        ratio[i] = c[i] / (1 - p[i]);
    }

    DisjointSets dsu(n + 1);
    vector<list<int>> seq(n + 1);
    for (int i = 1; i <= n; i++) seq[i].emplace_back(i);

    vector<bool> visited(n + 1, false);
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;
    for (int i = 1; i <= n; i++) pq.emplace(ratio[i], i);
    while (!pq.empty()) {
        auto [ai, i] = pq.top();
        pq.pop();

        if (visited[i]) continue;
        visited[i] = true;

        int d_set = dsu.find(d[i]), i_set = dsu.find(i);
        if (dsu.unite(d[i], i)) {
            seq[d_set].splice(seq[d_set].end(), seq[i_set]);
            c[d_set] += p[d_set] * c[i_set];
            p[d_set] *= p[i_set];
            ratio[d_set] = c[d_set] / (1 - p[d_set]);
            pq.emplace(ratio[d_set], d_set);
        }
    }
    for (int i : seq[0]) cout << i << "\n";
}
