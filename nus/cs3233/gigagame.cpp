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

    bool unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return false;

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        return true;
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

    vector<int> a(n), b(n);
    for (int &ai : a) {
        cin >> ai;
        ai--;
    }
    for (int &bi : b) {
        cin >> bi;
        bi--;
    }

    DisjointSets dsu(n);
    vector<int> count(n, 0);
    for (int i = 0; i < n; i++) {
        dsu.unite(a[i], b[i]);
        count[a[i]]++;
    }

    vector<int> twos(n, 0);
    for (int i = 0; i < n; i++)
        if (count[i] == 2) twos[dsu.find(i)]++;

    for (int i = 0; i < n; i++)
        if (dsu.find(i) == i && twos[i] >= 2) {
            cout << "NO";
            exit(0);
        }
    cout << "YES";
}