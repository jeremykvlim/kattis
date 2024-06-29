#include <bits/stdc++.h>
using namespace std;

struct DisjointSet {
    vector<int> sets;

    int find(int p) {
        return (sets[p] == p) ? p : (sets[p] = find(sets[p]));
    }

    bool unite(int p, int q) {
        int p_set = find(p), q_set = find(q);
        if (p_set != q_set) {
            sets[q_set] = p_set;
            return true;
        }
        return false;
    }

    DisjointSet(int n) : sets(n) {
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

    DisjointSet dsu(n);
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
