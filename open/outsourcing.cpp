#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int m1, n1, k1, m2, n2, k2;
        cin >> m1 >> n1 >> k1 >> m2 >> n2 >> k2;

        vector<vector<vector<int>>> adj_list1(251, vector<vector<int>>(251)), adj_list2(251, vector<vector<int>>(251));
        auto read = [&](int m, auto &adj_list) {
            while (m--) {
                int tin, tout, s;
                cin >> tin >> tout >> s;

                adj_list[tin][s].emplace_back(tout);
            }
        };
        read(m1, adj_list1);
        read(m2, adj_list2);

        vector<vector<bool>> visited(251, vector<bool>(251, false));
        auto dfs = [&](auto &&self, int v1 = 0, int v2 = 0) -> void {
            visited[v1][v2] = true;
            for (int k = 0; k <= 250; k++) {
                if (adj_list1[v1][k].empty()) adj_list1[v1][k].emplace_back(250);
                if (adj_list2[v2][k].empty()) adj_list2[v2][k].emplace_back(250);
                for (int u1 : adj_list1[v1][k])
                    for (int u2 : adj_list2[v2][k])
                        if (!visited[u1][u2]) self(self, u1, u2);
            }
        };
        dfs(dfs);

        for (int i = 0; i < n1 - 1; i++)
            if (visited[i][n2 - 1]) goto next;
        for (int i = 0; i < n2 - 1; i++)
            if (visited[n1 - 1][i]) goto next;
        if (visited[250][n2 - 1] || visited[n1 - 1][250]) goto next;

        cout << "eligible\n";
        continue;
        next:;
        cout << "not eligible\n";
    }
}
