#include <bits/stdc++.h>
using namespace std;

void tarjan(int v, vector<int> &order, vector<int> &low, vector<bool> &stacked, int &count, int &sccs, bool &valid, vector<vector<int>> &adj_list, stack<int> &s) {
    if (!valid) return;
    
    order[v] = low[v] = ++count;
    s.emplace(v);
    stacked[v] = true;

    int simple = 0;
    for (int &u : adj_list[v])
        if (!order[u]) {
            tarjan(u, order, low, stacked, count, sccs, valid, adj_list, s);
            low[v] = min(low[u], low[v]);
            if (low[u] != order[v]) simple++;
        } else if (stacked[u]) {
            low[v] = min(order[u], low[v]);
            simple++;
        }
    if (simple > 1) {
        valid = false;
        return;
    }
    
    if (order[v] == low[v]) {
        sccs++;
        for (int u = s.top(); u != v; u = s.top()) {
            s.pop();
            stacked[u] = false;
        }
        s.pop();
        stacked[v] = false;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj_list(n);
    while (m--) {
        int u, v;
        cin >> u >> v;

        adj_list[u].emplace_back(v);
    }

    vector<int> order(n, 0), low(n, 0);
    vector<bool> stacked(n, false);
    stack<int> s;
    int count = 0, sccs = 0;
    bool valid = true;
    for (int i = 0; i < n && valid; i++)
        if (!order[i]) tarjan(i, order, low, stacked, count, sccs, valid, adj_list, s);

    cout << (sccs == 1 && valid ? "YES" : "NO");
}
