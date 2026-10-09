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

    vector<int> a(n), depth(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];

        a[i]--;
        if (i) depth[i] = depth[a[i]] + 1;
    }

    vector<bool> cycle(n, false);
    for (int v = a[0]; v; v = a[v]) cycle[v] = true;
    cycle[0] = true;

    vector<long long> b(n);
    priority_queue<pair<long long, int>> antivirus;
    int infected = 0, cycle_infected = 0;
    for (int i = 0; i < n; i++) {
        cin >> b[i];

        if (b[i] > 0) antivirus.emplace(b[i], -i);
        if (b[i] < 0) {
            infected++;
            cycle_infected += cycle[i];
        }
    }

    DisjointSets dsu(n);
    vector<int> rep(n);
    iota(rep.begin(), rep.end(), 0);
    auto update = [&](int v) {
        int r = rep[dsu.find(a[v])];
        rep[dsu.unite(v, a[v]).first] = r;
    };
    for (int i = 1; i < n; i++)
        if (i != a[0] && !b[i]) update(i);

    auto peek = [&]() -> pair<long long, int> {
        while (!antivirus.empty()) {
            auto [bv, v] = antivirus.top();
            if (b[-v] == bv) return {bv, -v};
            antivirus.pop();
        }
        return {-1, -1};
    };

    auto day = 0LL;
    while (infected) {
        auto [bv, v] = peek();
        if (!~v) {
            cout << "never";
            exit(0);
        }
        antivirus.pop();

        int u = v ? rep[dsu.find(a[v])] : a[0];
        day += v ? depth[v] - depth[u] : 1;

        b[v] = 0;
        if (v && v != a[0]) update(v);
        b[u] += bv;

        if (b[u] > 0) antivirus.emplace(b[u], -u);
        if (b[u] >= 0 && b[u] < bv) {
            infected--;
            cycle_infected -= cycle[u];
            if (!b[u] && u && u != a[0]) update(u);
        }

        if (!v && infected && !cycle_infected && peek().second == u) {
            cout << "never";
            exit(0);
        }
    }
    cout << day;
}
