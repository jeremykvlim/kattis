#include <bits/stdc++.h>
using namespace std;

int find(int p, vector<int> &sets) {
    return (sets[p] == p) ? p : (sets[p] = find(sets[p], sets));
}

void dnc(int curr, int lg, vector<int> &days, vector<int> &cost, vector<int> &sets, vector<pair<int, int>> &roads, vector<pair<int, int>> &junctions) {
    if (lg < 0) {
        for (int d : days) cost[d] = curr;
        return;
    }

    int next = curr ^ (1 << lg);
    vector<int> undo;
    for (int i = next; i; --i &= next) {
        auto [u, v] = roads[i];
        int u_set = find(u, sets), v_set = find(v, sets);
        sets[v_set] = u_set;

        undo.emplace_back(u);
        undo.emplace_back(v);
    }

    vector<int> same, diff;
    for (int d : days) (find(junctions[d].first, sets) == find(junctions[d].second, sets) ? same : diff).emplace_back(d);

    for (int i : undo) sets[i] = i;
    dnc(next, lg - 1, same, cost, sets, roads, junctions);
    dnc(curr, lg - 1, diff, cost, sets, roads, junctions);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<int> sets(n + 1);
    iota(sets.begin(), sets.end(), 0);

    vector<pair<int, int>> roads(m);
    for (auto &[u, v] : roads) cin >> u >> v;
    roads.insert(roads.begin(), {0, 0});

    int q;
    cin >> q;

    vector<pair<int, int>> junctions(q);
    for (auto &[s, t] : junctions) cin >> s >> t;

    vector<int> days(q), cost(q);
    iota(days.begin(), days.end(), 0);
    dnc((1 << (__lg(m) + 1)) - 1, __lg(m), days, cost, sets, roads, junctions);

    for (int c : cost) cout << c << "\n";
}
