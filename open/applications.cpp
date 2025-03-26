#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets, size;
    int components;

    int find(int v) {
        return sets[v] == v ? v : (sets[v] = find(sets[v]));
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set != v_set) {
            sets[v_set] = u_set;
            size[u_set] += size[v_set];
            components--;
            return true;
        }
        return false;
    }

    DisjointSets(int n) : sets(n), size(n, 1), components(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int s, a;
    cin >> s >> a;

    DisjointSets dsu(s);
    while (a--) {
        int i, j, k;
        cin >> i >> j >> k;

        int t1 = dsu.find(i - 1), t2 = dsu.find(j - 1), t3 = dsu.find(k - 1),
            s1 = dsu.size[t1], s2 = dsu.size[t2], s3 = dsu.size[t3];

        if ((t1 == t2 && t2 == t3) || (s1 == s2 && s2 == s3) || (t1 == t2 && s1 > s3) ||
            (t1 == t3 && s1 > s2) || (t2 == t3 && s2 > s1)) {
            dsu.unite(t1, t2);
            dsu.unite(t1, t3);
        }
        cout << dsu.components << "\n";
    }
}
