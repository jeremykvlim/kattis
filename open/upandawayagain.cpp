#include <bits/stdc++.h>
using namespace std;

struct DisjointSets {
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

    DisjointSets(int n) : sets(n) {
        iota(sets.begin(), sets.end(), 0);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, x, t;
    cin >> n >> x >> t;

    int h_max = 0;
    vector<int> h(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> h[i];

        h_max = max(h_max, h[i]);
    }

    vector<int> d(n + 1);
    for (int i = 1; i <= n; i++) cin >> d[i];

    vector<vector<int>> indices(h_max + 2);
    for (int i = 1; i <= n; i++) indices[h[i]].emplace_back(i);

    DisjointSets dsu(h_max + 2);
    vector<long long> dist(n + 1, -1);
    dist[x] = 0;
    queue<int> q;
    q.emplace(x);
    while (!q.empty()) {
        int j = q.front();
        q.pop();

        int l = dsu.find(h[j]), r = min(h_max, h[j] + d[j]);
        for (int curr = l; curr <= r; curr = dsu.find(curr)) {
            for (int i : indices[curr])
                if (dist[i] == -1) {
                    dist[i] = dist[j] + 1;
                    q.emplace(i);
                }
            dsu.unite(curr + 1, curr);
        }
    }
    cout << (!~dist[1] ? dist[1] : dist[1] * t);
}
