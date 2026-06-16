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
    unordered_map<int, int> NE_SW, NW_SE;
    int count = n;
    for (int i = 0; i < n; i++) {
        int r, c;
        cin >> r >> c;

        for (int j = 0; j <= 2; j++) {
            if (NE_SW.count(r + c + j - 1))
                if (dsu.unite(i, NE_SW[r + c + j - 1])) count--;

            if (NW_SE.count(r - c + j - 1))
                if (dsu.unite(i, NW_SE[r - c + j - 1])) count--;
        }

        NE_SW[r + c] = NW_SE[r - c] = i;
    }

    cout << count;
}
