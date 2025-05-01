#include <bits/stdc++.h>
using namespace std;

pair<int, vector<int>> tarjan(int n, vector<vector<int>> &adj_list) {
    vector<int> order(n + 1, 0), low(n + 1, 0), component(n + 1, 0);
    vector<bool> stacked(n + 1, false);
    stack<int> st;
    int count = 0, sccs = 0;

    auto dfs = [&](auto &&self, int v) -> void {
        order[v] = low[v] = ++count;
        st.emplace(v);
        stacked[v] = true;

        for (int u : adj_list[v])
            if (!order[u]) {
                self(self, u);
                low[v] = min(low[v], low[u]);
            } else if (stacked[u]) low[v] = min(low[v], order[u]);

        if (order[v] == low[v]) {
            sccs++;
            int u;
            do {
                u = st.top();
                st.pop();
                stacked[u] = false;
                component[u] = sccs;
            } while (u != v);
        }
    };

    for (int v = 1; v <= n; v++)
        if (!order[v]) dfs(dfs, v);

    return {sccs, component};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    cin.ignore();

    vector<string> acronyms(n);
    for (auto &a : acronyms) getline(cin, a);

    unordered_map<string, int> compress;
    for (int i = 0; i < n; i++) compress[acronyms[i].substr(0, acronyms[i].find(' '))] = i + 1;

    vector<vector<int>> adj_list(n + 1);
    for (int i = 0; i < n; i++) {
        int s1 = acronyms[i].find(' '), s2 = acronyms[i].find(' ', s1 + 1),
            k = stoi(acronyms[i].substr(s1 + 1, s2 == string::npos ? string::npos : s2 - (s1 + 1))),
            j = s2 == string::npos ? acronyms[i].size() : s2 + 1;

        while (k--) {
            int s3 = acronyms[i].find(' ', j);
            auto word = s3 == string::npos ? acronyms[i].substr(j) : acronyms[i].substr(j, s3 - j);
            if (!word.empty() && isupper(word[0]))
                if (compress.count(word)) adj_list[i + 1].emplace_back(compress[word]);
            j = s3 == string::npos ? acronyms[i].size() : s3 + 1;
        }
    }

    auto [sccs, component] = tarjan(n, adj_list);

    vector<int> size(sccs + 1, 0);
    vector<vector<int>> dag(sccs + 1);
    for (int v = 1; v <= n; v++) {
        size[component[v]]++;
        for (int u : adj_list[v])
            if (component[v] != component[u]) dag[component[v]].emplace_back(component[u]);
    }

    for (auto &neighbours : dag) {
        sort(neighbours.begin(), neighbours.end());
        neighbours.erase(unique(neighbours.begin(), neighbours.end()), neighbours.end());
    }

    vector<int> order;
    vector<bool> visited(sccs + 1, false);
    auto dfs = [&](auto &&self, int v) -> void {
        visited[v] = true;
        for (int u : dag[v])
            if (!visited[u]) self(self, u);
        order.emplace_back(v);
    };
    for (int i = 1; i <= sccs; i++)
        if (!visited[i]) dfs(dfs, i);

    vector<bitset<(int) 3e4 + 1>> reach(sccs + 1);
    for (int v : order) {
        reach[v][v] = true;
        for (int u : dag[v]) reach[v] |= reach[u];
    }

    bool same = true;
    vector<pair<int, int>> groups;
    for (int v = 1; v <= sccs; v++) {
        if (size[v] != size[0]) same = false;
        if (size[v] > 1) groups.emplace_back(v, size[v] - 1);
    }

    vector<int> complexity(sccs + 1, 0);
    for (int v = 1; v <= sccs; v++) {
        complexity[v] = reach[v].count();

        if (same) complexity[v] *= size[0];
        else
            for (auto [u, s] : groups)
                if (reach[v][u]) complexity[v] += s;
    }

    for (int i = 1; i <= n; i++) cout << complexity[component[i]] << "\n";
}
