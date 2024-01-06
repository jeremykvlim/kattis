#include <bits/stdc++.h>
using namespace std;

int find(int p, vector<int> &sets) {
    return (sets[p] == p) ? p : (sets[p] = find(sets[p], sets));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, S;
    cin >> n >> m >> S;

    vector<vector<tuple<int, int, int>>> adj_list(n + 1);
    for (int i = 0; i < m; i++) {
        int a, b, c;
        cin >> a >> b >> c;

        adj_list[a].emplace_back(b, c, i);
        adj_list[b].emplace_back(a, c, i);
    }

    vector<bool> s(n + 1, false);
    vector<vector<tuple<int, int, int>>> curr(n + 1);
    while (S--) {
        s[1] = true;
        for (int i = 0; i < n - 1; i++) {
            int p;
            cin >> p;

            s[p] = true;
            for (auto [v, w, x] : adj_list[p])
                if (s[v]) curr[p].emplace_back(v, w, x);
        }

        for (int i = 1; i <= n; i++) {
            swap(adj_list[i], curr[i]);
            curr[i].clear();
            s[i] = false;
        }
    }

    vector<int> sets(n + 1), branches;
    iota(sets.begin(), sets.end(), 0);
    for (int u = 1; u <= n; u++) {
        tuple<int, int, int> best = {-1, -1, -1};

        for (auto [v, w, x] : adj_list[u])
            if (get<1>(best) < w) best = {v, w, x};

        if (get<2>(best) != -1) {
            sets[find(get<0>(best), sets)] = sets[find(u, sets)];
            branches.emplace_back(get<2>(best) + 1);
        }
    }

    if (branches.size() != n - 1) cout << "IMPOSSIBLE";
    else {
        sort(branches.begin(), branches.end());

        cout << n - 1 << "\n";
        for (auto b : branches) cout << b << " ";
    }
}
