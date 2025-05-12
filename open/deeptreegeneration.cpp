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

    vector<int> count(n + 1, 0), seen;
    for (int _ = 0; _ < n - 1; _++) {
        int p;
        cin >> p;

        if (!count[p]++) seen.emplace_back(p);
    }
    sort(seen.begin(), seen.end());
    int m = seen.size();

    DisjointSets dsu1(n + 2);
    for (int i = 2; i < n; i++)
        if (!count[i]) dsu1.unite(i + 1, i);

    vector<int> pos(m);
    for (int i = 0; i < m; i++) {
        pos[i] = dsu1.find(seen[i] + 1);
        dsu1.unite(pos[i] + 1, pos[i]);
    }

    DisjointSets dsu2(n + 2);
    for (int i : pos) dsu2.unite(i + 1, i);

    vector<int> q(n + 1);
    for (int i = 0; i < m; i++) q[pos[i]] = seen[i];

    for (int p : seen) count[p]--;
    for (int i = n - 1; i; i--)
        while (count[i]--) {
            int j = dsu2.find(i + 1);
            q[j] = i;
            dsu2.unite(j + 1, j);
        }

    for (int i = 2; i <= n; i++) cout << q[i] << " ";
}
