#include <bits/stdc++.h>
using namespace std;

void tarjan(int v, vector<int> &order, vector<int> &low, vector<int> &component, vector<vector<int>> &members, vector<bool> &stacked, int &count, int &sccs, vector<vector<int>> &adj_list, stack<int> &s) {
    order[v] = low[v] = ++count;
    s.emplace(v);
    stacked[v] = true;

    for (int &u : adj_list[v])
        if (!order[u]) {
            tarjan(u, order, low, component, members, stacked, count, sccs, adj_list, s);
            low[v] = min(low[v], low[u]);
        } else if (stacked[u]) low[v] = min(low[v], order[u]);

    if (order[v] == low[v]) {
        sccs++;
        if (members.size() == sccs) members.emplace_back();
        int u;
        do {
            u = s.top();
            s.pop();
            stacked[u] = false;
            component[u] = sccs;
            members[sccs].emplace_back(u);
        } while (u != v);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj_list(n + 1);
    while (m--) {
        int a, b;
        cin >> a >> b;

        adj_list[a].emplace_back(b);
    }

    vector<int> order(n + 1, 0), low(n + 1, 0), component(n + 1, 0);
    vector<vector<int>> members(1);
    vector<bool> stacked(n + 1, false);
    stack<int> st;
    int count = 0, sccs = 0;
    for (int i = 1; i <= n; i++)
        if (!order[i]) tarjan(i, order, low, component, members, stacked, count, sccs, adj_list, st);

    vector<int> dp1(n + 1, 0), dp2(n + 1, 0);
    for (int i = sccs; i; i--) {
        for (int v : members[i]) {
            dp1[v] = max(dp1[v], 1);
            dp2[v] = max(dp2[v], 1);
        }

        int s = members[i].size();
        vector<vector<bool>> same(s, vector<bool>(s, false));
        for (int v : members[i])
            for (int u : adj_list[v])
                if (component[v] == component[u]) {
                    int j = find(members[i].begin(), members[i].end(), v) - members[i].begin(),
                        k = find(members[i].begin(), members[i].end(), u) - members[i].begin();

                    same[j][k] = true;
                }

        vector<int> menu(s);
        iota(menu.begin(), menu.end(), 0);
        do {
            for (int j = 0, k = 0;; j++, k++) {
                int u = members[i][menu[j]];
                if (!j) k = dp2[u];

                dp1[u] = max(dp1[u], k);

                if (j == s - 1 || !same[menu[j]][menu[j + 1]]) break;
            }
        } while (next_permutation(menu.begin(), menu.end()));

        for (int v : members[i])
            for (int u : adj_list[v])
                if (component[v] != component[u]) dp2[u] = max(dp2[u], dp1[v] + 1);
    }

    cout << *max_element(dp1.begin(), dp1.end());
}
