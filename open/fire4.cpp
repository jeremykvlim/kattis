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

vector<pair<int, int>> monotone_jumps(const vector<int> &next, const vector<pair<int, int>> &queries) {
    int n = next.size();
    vector<vector<int>> adj_list(n + 1);
    vector<int> depth(n + 1, 0);
    for (int v = 0; v < n; v++) adj_list[next[v]].emplace_back(v);
    for (int v = n - 1; ~v; v--) depth[v] = depth[next[v]] + 1;

    DisjointSets dsu(n + 1);
    vector<int> rep(n + 1);
    iota(rep.begin(), rep.end(), 0);

    int q = queries.size();
    vector<pair<int, int>> result(q);
    for (int i = 0, v = 0; i < q; i++) {
        auto [a, bound] = queries[i];
        for (; v <= bound; v++)
            for (int u : adj_list[v]) {
                auto [big, small] = dsu.unite(u, v);
                if (small != -1) rep[big] = max(rep[big], rep[small]);
            }

        int b = rep[dsu.find(a)];
        result[i] = {depth[a] - depth[b], b};
    }
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<pair<int, int>> intervals, overnight;
    while (n--) {
        int s, e;
        cin >> s >> e;

        if (s < e) intervals.emplace_back(s, e);
        else overnight.emplace_back(s, e);
    }
    if (overnight.empty()) {
        cout << -1;
        exit(0);
    }

    sort(intervals.begin(), intervals.end(), [&](auto p1, auto p2) { return p1.first != p2.first ? p1.first < p2.first : p1.second > p2.second; });
    vector<int> l, r;
    for (auto [s, e] : intervals)
        if (r.empty() || e > r.back()) {
            l.emplace_back(s);
            r.emplace_back(e);
        }

    int k = l.size();
    vector<int> next(k, k);
    for (int i = 0, j = 0; i < k; i++) {
        j = max(j, i);
        for (; j + 1 < k && l[j + 1] <= r[i]; j++);
        if (i < j) next[i] = j;
    }

    int least = INT_MAX;
    vector<pair<int, int>> queries;
    vector<int> starts;
    sort(overnight.begin(), overnight.end());
    for (auto [s, e] : overnight) {
        int i = upper_bound(l.begin(), l.end(), e) - l.begin() - 1;
        if (!~i || r[i] <= e) continue;
        if (r[i] >= s) {
            least = min(least, 2);
            continue;
        }

        queries.emplace_back(i, lower_bound(r.begin(), r.end(), s) - r.begin() - 1);
        starts.emplace_back(s);
    }

    auto result = monotone_jumps(next, queries);
    for (int i = 0; i < result.size(); i++) {
        auto [cost, v] = result[i];
        int u = next[v];
        if (u < k && r[u] >= starts[i]) least = min(least, cost + 3);
    }
    cout << (least == INT_MAX ? -1 : least);
}