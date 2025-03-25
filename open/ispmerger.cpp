#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets, size;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
            size[u_set] += size[v_set];
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n), size(n, 1) {
        iota(sets.begin(), sets.end(), 0);
    }
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

        int u_set = dsu.find(u), v_set = dsu.find(v);
        if (dsu.unite(u, v)) {
            capacity[u_set] += capacity[v_set];
            links[u_set] += links[v_set];
        }

        capacity[u_set] -= 2;
        links[u_set]++;
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

            if (links[s] == dsu.size[s] - 1) complete += 2;
            else isolated++;

            degree += 2;
        } else if (capacity[s] == 1) {
            if (links[s] == dsu.size[s] - 1) complete++;
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
