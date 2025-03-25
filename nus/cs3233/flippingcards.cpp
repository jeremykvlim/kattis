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

    int t;
    cin >> t;

    while (t--) {
        int n;
        cin >> n;

        DisjointSets dsu(2 * (n + 1));
        vector<bool> visited(2 * (n + 1), false);

        bool possible = true;
        while (n--) {
            int p, q;
            cin >> p >> q;

            if (!possible) continue;

            int p_set = dsu.find(p), q_set = dsu.find(q);
            if (visited[p_set] && visited[q_set]) possible = false;

            visited[p_set] = dsu.unite(p, q) ? visited[p_set] | visited[q_set] : true;
        }

        cout << (possible ? "possible\n" : "impossible\n");
    }
}
