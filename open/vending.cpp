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
        for (int u = s.top(); u != v; u = s.top()) {
            s.pop();
            stacked[u] = false;
            component[u] = sccs;
        }
        s.pop();
        stacked[v] = false;
        component[v] = sccs;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> f(n + 1), p(n + 1), m(n + 1), s(n + 1);
    for (int i = 1; i <= n; i++) cin >> f[i] >> p[i] >> m[i] >> s[i];

    vector<vector<int>> adj_list(n + 1);
    unordered_set<int> cheaper;
    for (int i = 1; i <= n; i++)
        if (m[f[i]] > p[i]) {
            cheaper.emplace(i);
            adj_list[i].emplace_back(f[i]);
        }

    vector<int> order(n + 1, 0), low(n + 1, 0), component(n + 1, 0);
    vector<bool> stacked(n + 1, false);
    stack<int> st;
    int count = 0, sccs = 0;

    for (int i = 1; i <= n; i++)
        if (!order[i]) tarjan(i, order, low, component, stacked, count, sccs, adj_list, st);

    vector<int> same(n + 1, 0), diff(n + 1, 0), sales(n + 1, INT_MIN);
    for (int i : cheaper)
        if (component[i] == component[f[i]]) same[f[i]] = max(same[f[i]], m[f[i]] - p[i]);
        else diff[f[i]] = max(diff[f[i]], m[f[i]] - p[i]);

    vector<bool> visited(n + 1, false);
    auto profit = 0LL;
    for (int i = 1; i <= n; i++) {
        profit += (long long) s[i] * max(same[i], diff[i]);

        if (i == f[i] || same[i] <= diff[i]) visited[component[i]] = true;
        else sales[component[i]] = max(sales[component[i]], diff[i] - same[i]);
    }

    for (int i = 1; i <= sccs; i++)
        if (!visited[i]) profit += sales[i];

    cout << profit;
}
