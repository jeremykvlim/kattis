#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, c, s, f;
    string sigma;
    cin >> n >> c >> s >> f >> sigma;

    if (!f) {
        cout << "finite";
        exit(0);
    }

    vector<int> final(f);
    for (int &fi : final) cin >> fi;

    vector<vector<int>> table(n + 1, vector<int>(c)), indices(n + 1);
    for (int i = 1; i <= n; i++)
        for (int &symb : table[i]) {
            cin >> symb;

            indices[symb].emplace_back(i);
        }

    vector<bool> visited1(n + 1, false), visited2(n + 1, false);
    visited1[s] = true;
    queue<int> q;
    q.emplace(s);
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (int u : table[v])
            if (!visited1[u]) {
                visited1[u] = true;
                q.emplace(u);
            }
    }

    for (int fi : final) {
        if (visited1[fi] && !visited2[fi]) {
            visited2[fi] = true;
            q.emplace(fi);
        }
    }

    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (int u : indices[v]) {
            if (visited1[u] && !visited2[u]) {
                visited2[u] = true;
                q.emplace(u);
            }
        }
    }

    if (all_of(final.begin(), final.end(), [&](int fi) {return !visited1[fi] || !visited2[fi];})) {
        cout << "finite";
        exit(0);
    }

    vector<int> state(n + 1, 0);
    vector<vector<int>> adj_list(n + 1);
    for (int v = 1; v <= n; v++)
        if (visited1[v] && visited2[v])
            for (int u : table[v])
                if (visited1[u] && visited2[u]) adj_list[v].emplace_back(u);

    auto dfs = [&](auto &&self, int v) -> bool {
        state[v] = 1;
        for (int u : adj_list[v]) 
            if (!state[u]) {
                if (self(self, u)) return true;
            } else if (state[u] == 1) return true;
        
        state[v] = 2;
        return false;
    };

    for (int v = 1; v <= n; v++)
        if (visited1[v] && visited2[v] && !state[v])
            if (dfs(dfs, v)) {
                cout << "infinite";
                exit(0);
            }
    cout << "finite";
}
