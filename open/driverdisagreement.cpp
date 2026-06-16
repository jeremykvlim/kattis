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

    int n, A, B;
    cin >> n >> A >> B;

    vector<int> l(n), r(n), t(n);
    for (int i = 0; i < n; i++) cin >> l[i] >> r[i] >> t[i];

    DisjointSets dsu(n);
    queue<array<int, 3>> q;
    q.push({A, B, 0});
    while (!q.empty()) {
        auto [a, b, d] = q.front();
        q.pop();

        if (t[a] + t[b] == 1) {
            cout << d;
            exit(0);
        }

        if (dsu.unite(a, b)) {
            q.push({l[a], l[b], d + 1});
            q.push({r[a], r[b], d + 1});
        }
    }

    cout << "indistinguishable";
}
