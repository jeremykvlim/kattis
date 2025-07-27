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

    int n, A, B;
    cin >> n >> A >> B;

    vector<int> l(n), r(n), t(n);
    for (int i = 0; i < n; i++) cin >> l[i] >> r[i] >> t[i];

    DisjointSets dsu(n);
    queue<array<int, 3>> q;
    q.emplace(A, B, 0);
    while (!q.empty()) {
        auto [a, b, d] = q.front();
        q.pop();

        if (t[a] + t[b] == 1) {
            cout << d;
            exit(0);
        }

        if (dsu.unite(a, b)) {
            q.emplace(l[a], l[b], d + 1);
            q.emplace(r[a], r[b], d + 1);
        }
    }

    cout << "indistinguishable";
}
