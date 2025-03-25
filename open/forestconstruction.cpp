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

    int V;
    cin >> V;

    if (!V) {
        cout << "POSSIBLE";
        exit(0);
    }

    vector<int> degrees(V);
    long long sum_d = 0;
    for (int &d : degrees) {
        cin >> d;

        sum_d += d;
    }

    if (sum_d & 1 || sum_d > 2 * (V - 1)) {
        cout << "IMPOSSIBLE";
        exit(0);
    }

    vector<pair<int, int>> vertices;
    for (int i = 1; i <= V; i++) vertices.emplace_back(i, degrees[i - 1]);
    sort(vertices.begin(), vertices.end(), [&](auto v1, auto v2) {return v1.second != v2.second ? v1.second > v2.second : v1.first < v2.first;});

    DisjointSets dsu(V + 1);
    vector<pair<int, int>> edges;
    while (vertices[0].second) {
        auto [u, du] = vertices[0];
        vector<int> candidates;
        for (int i = 1; i < vertices.size(); i++) {
            auto [v, dv] = vertices[i];
            if (dsu.find(u) != dsu.find(v)) candidates.emplace_back(i);
        }

        if (candidates.size() < du) {
            cout << "IMPOSSIBLE";
            exit(0);
        }

        for (int i = 0; i < du; i++) {
            auto [v, dv] = vertices[candidates[i]];

            edges.emplace_back(u, v);
            vertices[0].second--;
            vertices[candidates[i]].second--;
            dsu.unite(u, v);
        }
        sort(vertices.begin(), vertices.end(), [&](auto v1, auto v2) {return v1.second != v2.second ? v1.second > v2.second : v1.first < v2.first;});
    }

    if (any_of(vertices.begin(), vertices.end(), [&](auto v) {return v.second;})) {
        cout << "IMPOSSIBLE";
        exit(0);
    }

    cout << "POSSIBLE\n";
    for (auto [a, b] : edges) cout << a << " " << b << "\n";
}
