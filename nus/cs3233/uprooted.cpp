#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, S;
    cin >> n >> m >> S;

    vector<vector<array<int, 3>>> adj_list(n + 1);
    for (int i = 1; i <= m; i++) {
        int a, b, c;
        cin >> a >> b >> c;

        adj_list[a].push_back({b, c, i});
        adj_list[b].push_back({a, c, i});
    }

    vector<bool> s(n + 1, false);
    vector<vector<array<int, 3>>> curr(n + 1);
    while (S--) {
        s[1] = true;
        for (int _ = 0; _ < n - 1; _++) {
            int p;
            cin >> p;

            s[p] = true;
            for (auto [q, r, i] : adj_list[p])
                if (s[q]) curr[p].push_back({q, r, i});
        }

        for (int i = 1; i <= n; i++) {
            swap(adj_list[i], curr[i]);
            curr[i].clear();
            s[i] = false;
        }
    }

    vector<int> branches;
    for (int p = 1; p <= n; p++) {
        array<int, 3> best{-1, -1, -1};

        for (auto [q, r, i] : adj_list[p])
            if (best[1] < r) best = {q, r, i};

        if (best[0] != -1) branches.emplace_back(best[2]);
    }

    if (branches.size() != n - 1) cout << "IMPOSSIBLE";
    else {
        sort(branches.begin(), branches.end());

        cout << n - 1 << "\n";
        for (int b : branches) cout << b << " ";
    }
}
