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