#include <bits/stdc++.h>
using namespace std;

void tarjan(int v, vector<int> &order, vector<int> &low, vector<int> &component, vector<bool> &stacked, int &count, int &sccs, vector<vector<int>> &adj_list, stack<int> &s) {
    order[v] = low[v] = ++count;
    s.emplace(v);
    stacked[v] = true;

    for (int &u : adj_list[v])
        if (!order[u]) {
            tarjan(u, order, low, component, stacked, count, sccs, adj_list, s);
            low[v] = min(low[u], low[v]);
        } else if (stacked[u]) low[v] = min(order[u], low[v]);

    if (order[v] == low[v]) {
        sccs++;
        int u;
        do {
            u = s.top();
            s.pop();
            stacked[u] = false;
            component[u] = sccs;
        } while (u != v);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;

    while (t--) {
        int n, m;
        cin >> n >> m;

        if (!m) {
            cout << n << "\n";
            continue;
        }

        vector<vector<int>> adj_list(n + 1);
        while (m--) {
            int s1, s2;
            cin >> s1 >> s2;

            adj_list[s1].emplace_back(s2);
        }

        vector<int> order(n + 1, 0), low(n + 1, 0), component(n + 1, 0);
        vector<bool> stacked(n + 1, false);
        stack<int> st;
        int count = 0, sccs = 0;

        for (int i = 1; i <= n; i++)
            if (!order[i]) tarjan(i, order, low, component, stacked, count, sccs, adj_list, st);

        if (sccs == 1) {
            cout << "0\n";
            continue;
        }

        vector<int> in(sccs + 1, 0), out(sccs + 1, 0);
        for (int v = 1; v <= n; v++)
            for (int u : adj_list[v])
                if (component[u] != component[v]) {
                    in[component[u]]++;
                    out[component[v]]++;
                }

        cout << max(count_if(in.begin() + 1, in.end(), [](int d) {return !d;}),
                    count_if(out.begin() + 1, out.end(), [](int d) {return !d;})) << "\n";
    }
}
