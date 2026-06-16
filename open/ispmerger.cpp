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

    int n, m, k;
    cin >> n >> m >> k;

    vector<int> capacity(n), links(n, 0);
    for (int &c : capacity) cin >> c;

    if (accumulate(capacity.begin(), capacity.end(), 0) < 2 * (n - 1)) {
        cout << "no";
        exit(0);
    }

    DisjointSets dsu(n);
    while (m--) {
        int u, v;
        cin >> u >> v;

        auto [big, small] = dsu.unite(u, v);
        if (small != -1) {
            capacity[big] += capacity[small];
            links[big] += links[small];
        }

        capacity[big] -= 2;
        links[big]++;
    }

    vector<int> roots;
    for (int i = 0; i < n; i++)
        if (i == dsu.find(i)) roots.emplace_back(i);

    if (roots.size() == 1) {
        cout << "yes";
        exit(0);
    }

    int isolated = 0, complete = 0, incomplete = 0, degree = 0;
    for (int s : roots) {
        degree += capacity[s];

        if (!capacity[s]) {
            k--;

            if (links[s] == dsu.size(s) - 1) complete += 2;
            else isolated++;

            degree += 2;
        } else if (capacity[s] == 1) {
            if (links[s] == dsu.size(s) - 1) complete++;
            else incomplete++;
        } else isolated++;
    }

    while (degree < 2 * (complete + incomplete)) {
        k--;
        degree += 2;

        if (incomplete) {
            incomplete--;
            isolated++;
        }
    }

    cout << (k < isolated + complete + incomplete - 1 ? "no" : "yes");
}