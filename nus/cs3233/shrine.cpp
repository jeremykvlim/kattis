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

    int w;
    while (cin >> w && w) {
        int n, D;
        cin >> n >> D;

        vector<int> shrines;
        while (D--) {
            int d;
            cin >> d;

            for (int i = d; i <= n; i += d) shrines.emplace_back(i);
        }
        sort(shrines.begin(), shrines.end());
        shrines.erase(unique(shrines.begin(), shrines.end()), shrines.end());

        if (shrines.size() <= w) {
            cout << "2000.0\n";
            continue;
        }

        int s = shrines.size();
        double R = 1e3;
        auto law_of_cosines = [&](int d) {
            return sqrt(R * R + R * R - 2 * R * R * cos(2 * M_PI * d / n));
        };

        vector<double> dist{law_of_cosines(shrines[0])};
        for (int i = 0; i < s - 1; i++) dist.emplace_back(law_of_cosines(shrines[i + 1] - shrines[i]));

        int k = 2 * s;
        vector<double> pref(k + 1);
        for (int i = 0; i < k; i++) pref[i + 1] = pref[i] + dist[i % s];

        double l = 2 * R, r = 2 * M_PI * R + 2 * R, m;
        while (l + 1e-6 < r && l + l * 1e-6 < r) {
            m = l + (r - l) / 2;

            auto valid = [&]() {
                vector<int> next(k, k);
                for (int i = 0, j = 1; i < k; i++) {
                    j = max(j, i + 1);
                    for (; j < k && 2 * R + pref[j] - pref[i] <= m; j++);
                    next[i] = j;
                }

                vector<pair<int, int>> queries(s);
                for (int i = 0; i < s; i++) queries[i] = {i, i + s - 1};
                for (auto [cost, v] : monotone_jumps(next, queries))
                    if (cost + 1 <= w) return true;

                return false;
            };

            if (valid()) r = m;
            else l = m;
        }
        cout << fixed << setprecision(1) << r << "\n";
    }
}
