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

    DisjointSets dsu(n);
    vector<int> count(n, 1);
    unordered_map<int, int> seen;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;

        auto it = seen.find(x);
        if (it == seen.end()) seen[x] = i;
        else {
            int i_set = dsu.find(i), j_set = dsu.find(it->second);
            if (dsu.unite(i_set, j_set)) count[dsu.find(j_set)] = count[i_set] + count[j_set];
        }
    }

    int q;
    cin >> q;

    while (q--) {
        int i, j;
        cin >> i >> j;
        i--;
        j--;

        int i_set = dsu.find(i), j_set = dsu.find(j);
        if (i_set == j_set) {
            cout << count[i_set] << " " << count[i_set] << "\n";
            continue;
        }

        cout << count[i_set] << " " << count[i_set] + count[j_set] << "\n";
        dsu.unite(j_set, i_set);
        count[dsu.find(j_set)] = count[i_set] + count[j_set];
    }
}
