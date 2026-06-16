#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
    vector<int> sets;
    int components;

    int find(int v) {
        while (sets[v] >= 0) {
            int p = sets[v];
            if (sets[p] >= 0) sets[v] = sets[p];
            v = p;
        }
        return v;
    }

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return false;

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        components--;
        return true;
    }

    int size(int v) {
        return -sets[find(v)];
    }

    DisjointSets(int n) : sets(n, -1), components(n) {}
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
            s1 = dsu.size(t1), s2 = dsu.size(t2), s3 = dsu.size(t3);

        if ((t1 == t2 && t2 == t3) || (s1 == s2 && s2 == s3) || (t1 == t2 && s1 > s3) ||
            (t1 == t3 && s1 > s2) || (t2 == t3 && s2 > s1)) {
            dsu.unite(t1, t2);
            dsu.unite(t1, t3);
        }
        cout << dsu.components << "\n";
    }
}