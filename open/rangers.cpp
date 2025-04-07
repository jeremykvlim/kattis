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

tuple<vector<pair<int, int>>, vector<vector<bool>>> kruskal(int n, vector<array<int, 3>> edges) {
    DisjointSets dsu(n);
    sort(edges.begin(), edges.end());

    vector<pair<int, int>> mst;
    vector<vector<bool>> in_mst(n, vector<bool>(n, false));
    for (auto [w, u, v] : edges)
        if (dsu.unite(u, v)) {
            mst.emplace_back(u, v);
            in_mst[u][v] = in_mst[v][u] = true;
        }

    return {mst, in_mst};
}

vector<complex<double>> quadratic_roots(double a, double b, double c) {
    if (fabs(a) < 1e-8 && fabs(b) < 1e-8) return {};
    if (fabs(a) < 1e-8) return {(-c / b)};

    complex<double> discriminant(b * b - 4 * a * c, 0);
    return {(-b + sqrt(discriminant)) / (2 * a), (-b - sqrt(discriminant)) / (2 * a)};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    for (int n, tc = 1; cin >> n; tc++) {
        vector<array<int, 6>> bases(n);
        for (auto &[x, y, z, vx, vy, vz] : bases) cin >> x >> y >> z >> vx >> vy >> vz;

        vector<vector<array<int, 3>>> eqns(n, vector<array<int, 3>>(n, {0, 0, 0}));
        for (int i = 0; i < n; i++)
            for (int j = 0; j < i; j++)
                for (int d = 0; d < 3; d++) {
                    int x = bases[i][d + 3] - bases[j][d + 3], y = bases[i][d] - bases[j][d];
                    eqns[i][j][0] += x * x;
                    eqns[i][j][1] += 2 * x * y;
                    eqns[i][j][2] += y * y;
                }

        vector<pair<double, array<int, 4>>> events;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < i; j++) {
                for (int k = 0; k < i; k++)
                    for (int l = 0; l < k; l++) {
                        auto roots = quadratic_roots(eqns[i][j][0] - eqns[k][l][0], eqns[i][j][1] - eqns[k][l][1], eqns[i][j][2] - eqns[k][l][2]);
                        for (auto x : roots)
                            if (x.imag() == 0 && x.real() >= 0) events.emplace_back(x.real(), array<int, 4>{i, j, k, l});
                    }

                for (int l = 0; l < j; l++) {
                    auto roots = quadratic_roots(eqns[i][j][0] - eqns[i][l][0], eqns[i][j][1] - eqns[i][l][1], eqns[i][j][2] - eqns[i][l][2]);
                    for (auto x : roots)
                        if (x.imag() == 0 && x.real() >= 0) events.emplace_back(x.real(), array<int, 4>{i, j, i, l});
                }
            }

        auto quadratic_slope = [&](auto a, auto b, auto x) {
            return 2 * a * x + b;
        };
        for (auto &[x, indices] : events) {
            auto &[i, j, k, l] = indices;
            if (quadratic_slope(eqns[i][j][0], eqns[i][j][1], x) < quadratic_slope(eqns[k][l][0], eqns[k][l][1], x)) {
                swap(i, k);
                swap(j, l);
            }
        }
        sort(events.begin(), events.end());
        
        auto eval = [&](auto a, auto b, auto c, auto x) -> int {
            return a * x * x + b * x + c;
        };
        vector<array<int, 3>> edges;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < i; j++) edges.push_back({eval(eqns[i][j][0], eqns[i][j][1], eqns[i][j][2], 0), i, j});
        auto [mst, in_mst] = kruskal(n, edges);

        int count = 1;
        for (int e = 0; e < events.size();) {
            auto t = events[e].first;
            while (e < events.size() && events[e].first < t + 1e-6) {
                auto [x, indices] = events[e++];
                auto [i, j, k, l] = indices;
                if (!(in_mst[i][j] && in_mst[j][i]) || in_mst[k][l] && in_mst[l][k]) continue;

                for (auto it = mst.begin(); it != mst.end(); it++) {
                    auto [u, v] = *it;
                    if (u == i && v == j) {
                        mst.erase(it);
                        break;
                    }
                }
                if (mst.size() == n - 1) continue;
                in_mst[i][j] = in_mst[j][i] = false;

                DisjointSets dsu(n);
                for (auto [u, v] : mst) dsu.unite(u, v);

                if (dsu.find(k) != dsu.find(l)) {
                    mst.emplace_back(k, l);
                    in_mst[k][l] = in_mst[l][k] = true;
                    count++;
                } else {
                    mst.emplace_back(i, j);
                    in_mst[i][j] = in_mst[j][i] = true;
                }
            }
        }
        cout << "Case " << tc << ": " << count << "\n";
    }
}
