#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, m;
        cin >> n >> m;

        vector<vector<int>> adj_list(n);
        for (int i = 1; i < n; i++) {
            int p;
            cin >> p;

            adj_list[p - 1].emplace_back(i);
        }

        int h = 0;
        vector<int> depth(n, 0);
        vector<long long> count(n + m + 1, 0);
        count[0] = 1;
        queue<int> q;
        q.emplace(0);
        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (int u : adj_list[v]) {
                q.emplace(u);
                count[depth[u] = depth[v] + 1]++;
                h = max(h, depth[u]);
            }
        }

        vector<int> active;
        for (int d = h; d; d--) active.emplace_back(d);
        bool possible = true;
        while (m--) {
            int a, b;
            cin >> a >> b;

            if (!possible) continue;

            count[++h] = count[h];
            vector<int> temp;
            if (count[h] < 1e18) temp.emplace_back(h);
            for (int d : active)
                if (d < h) {
                    count[d] += count[d - 1];
                    if (count[d] < 1e18) temp.emplace_back(d);
                }

            while (h && (h > b || a >= count[h])) {
                a -= min((long long) a, count[h]);
                count[h--] = 0;
            }

            if (a) {
                if (!h) possible = false;
                else count[h] -= a;
            }
            active = temp;
        }
        cout << (possible ? "yes\n" : "no\n");
    }
}
