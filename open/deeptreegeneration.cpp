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

    pair<int, int> unite(int u, int v) {
        int u_set = find(u), v_set = find(v);
        if (u_set == v_set) return {u_set, -1};

        if (sets[u_set] > sets[v_set]) swap(u_set, v_set);
        sets[u_set] += sets[v_set];
        sets[v_set] = u_set;
        return {u_set, v_set};
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

    vector<int> count(n + 1, 0), seen;
    for (int _ = 0; _ < n - 1; _++) {
        int p;
        cin >> p;

        if (!count[p]++) seen.emplace_back(p);
    }
    sort(seen.begin(), seen.end());
    int m = seen.size();

    DisjointSets dsu1(n + 2);
    vector<int> rep1(n + 2);
    iota(rep1.begin(), rep1.end(), 0);

    for (int i = 2; i < n; i++)
        if (!count[i]) {
            auto [big, small] = dsu1.unite(i + 1, i);
            if (small != -1) rep1[big] = max(rep1[big], rep1[small]);
        }

    vector<int> pos(m);
    for (int i = 0; i < m; i++) {
        pos[i] = rep1[dsu1.find(seen[i] + 1)];

        auto [big, small] = dsu1.unite(pos[i] + 1, pos[i]);
        if (small != -1) rep1[big] = max(rep1[big], rep1[small]);
    }

    DisjointSets dsu2(n + 2);
    vector<int> rep2(n + 2);
    iota(rep2.begin(), rep2.end(), 0);

    for (int i : pos) {
        auto [big, small] = dsu2.unite(i + 1, i);
        if (small != -1) rep2[big] = max(rep2[big], rep2[small]);
    }

    vector<int> q(n + 1);
    for (int i = 0; i < m; i++) q[pos[i]] = seen[i];

    for (int p : seen) count[p]--;
    for (int i = n - 1; i; i--)
        while (count[i]--) {
            int j = rep2[dsu2.find(i + 1)];
            q[j] = i;

            auto [big, small] = dsu2.unite(j + 1, j);
            if (small != -1) rep2[big] = max(rep2[big], rep2[small]);
        }

    for (int i = 2; i <= n; i++) cout << q[i] << " ";
}